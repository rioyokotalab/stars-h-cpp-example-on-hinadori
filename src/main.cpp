#include <iostream>
#include <memory>
#include <lapacke.h>
#include <cblas.h>
#include <mpi.h>
extern "C" {
#include <starsh.h>
#include <starsh-minimal.h>
}

constexpr std::size_t N = 1lu << 2;
constexpr std::size_t block_size = 1lu << 7;
constexpr std::size_t rank = 1lu << 7;
constexpr double tol = 8.;

#define STARSH_CHECK_ERROR(x) if((x) != 0) {std::printf("Error@%s[%s, l.%d], [code=%d]\n", __func__, __FILE__, __LINE__, (x));}

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	const char dtype = 's';
	const char symm = 'N';
	const int ndim = 2;
	const int onfly = 0;
	STARSH_int shape[2] = {N, N};
	STARSH_CHECK_ERROR(starsh_init());

	STARSH_mindata *data;
	STARSH_kernel *kernel;

	STARSH_CHECK_ERROR(starsh_application(
		reinterpret_cast<void**>(&data), &kernel, N, dtype,
		STARSH_MINIMAL, STARSH_MINIMAL_KERNEL1, 0));

	STARSH_problem *problem;
	const std::string problem_name = "Minimal example (SP)";
	STARSH_CHECK_ERROR(starsh_problem_new(&problem, ndim, shape, symm, dtype, data, data, kernel, problem_name.c_str()));

	Array *array;
	STARSH_CHECK_ERROR(starsh_problem_to_array(problem, &array));

	for (unsigned i = 0; i < N; i++) {
		for (unsigned j = 0; j < N; j++) {
			std::printf("%+e ", static_cast<double*>(array->data)[i + N * j]);
		}
		std::printf("\n");
	}

	starsh_problem_free(problem);
	array_free(array);

	MPI_Finalize();

	return 0;
}
