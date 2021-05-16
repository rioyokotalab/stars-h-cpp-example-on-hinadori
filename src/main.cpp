#include <iostream>
#include <memory>
#include <lapacke.h>
#include <cblas.h>
#include <mpi.h>
extern "C" {
#include <starsh.h>
#include <starsh-randtlr.h>
}

constexpr std::size_t N = 1lu << 5;
constexpr std::size_t rank = N / 2;
constexpr int block_size = N / 2;

#define STARSH_CHECK_ERROR(x) if((x) != 0) {std::printf("Error@%s[%s, l.%d], [code=%d]\n", __func__, __FILE__, __LINE__, (x));}

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	const char dtype = 'd';
	const char symm = 'N';
	const int ndim = 2;
	const int onfly = 0;
	const double diag = N;
	const double decay = 0.5;
	STARSH_int shape[2] = {N, N};
	STARSH_CHECK_ERROR(starsh_init());

	STARSH_randtlr *data;
	STARSH_kernel *kernel;

	int iseed[4] = {0, 0, 0, 4};
	starsh_set_seed(iseed);

	for (unsigned i = 0; i < 10; i++) {
		STARSH_CHECK_ERROR(starsh_application(
				reinterpret_cast<void**>(&data), &kernel, N, dtype,
				STARSH_RANDTLR, STARSH_RANDTLR_KERNEL1,
				STARSH_RANDTLR_NB, block_size,
				STARSH_RANDTLR_DECAY, decay,
				STARSH_RANDTLR_DIAG, diag,
				0));
		STARSH_problem *problem;
		Array *array;
		std::printf("ID = %u\n", i);
		const std::string problem_name = "Minimal example (SP)";
		STARSH_CHECK_ERROR(starsh_problem_new(&problem, ndim, shape, symm, dtype, data, data, kernel, problem_name.c_str()));

		starsh_problem_info(problem);

		STARSH_CHECK_ERROR(starsh_problem_to_array(problem, &array));

		for (unsigned i = 0; i < N; i++) {
			for (unsigned j = 0; j < N; j++) {
				std::printf("%+e ", static_cast<double*>(array->data)[i + N * j]);
			}
			std::printf("\n");
		}

		std::fflush(stdout);
		array_free(array);
		starsh_problem_free(problem);
	}

	MPI_Finalize();

	return 0;
}
