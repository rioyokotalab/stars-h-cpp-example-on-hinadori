#include <iostream>
extern "C" {
#include <starsh.h>
#include <starsh-minimal.h>
}

constexpr std::size_t N = 1lu << 10;
constexpr std::size_t block_size = 1lu << 7;
constexpr std::size_t rank = 1lu << 7;
constexpr double tol = 8.;

#define STARSH_CHECK_ERROR(x) if((x) != 0) {std::printf("Error@%s[%s, l.%d], [code=%d]\n", __func__, __FILE__, __LINE__, (x));}

int main() {
	const char dtype = 's';
	const char symm = 'N';
	const int ndim = 2;
	STARSH_int shape[2] = {N, N};
	STARSH_CHECK_ERROR(starsh_init());

	STARSH_mindata *data;
	STARSH_kernel *kernel;

	STARSH_CHECK_ERROR(starsh_application(
		reinterpret_cast<void**>(&data), &kernel, N, dtype,
		STARSH_MINIMAL, STARSH_MINIMAL_KERNEL1, 0));

	STARSH_problem *problem;
	const std::string problem_name = "Minimal example (SP)";
	STARSH_CHECK_ERROR(starsh_problem_new(&problem, ndim, shape, symm, dtype, data, data, kernel, const_cast<char*>(problem_name.c_str())));

	starsh_problem_info(problem);

	STARSH_cluster *cluster;
	STARSH_CHECK_ERROR(starsh_cluster_new_plain(&cluster, data, N, block_size));

	starsh_cluster_info(cluster);
}
