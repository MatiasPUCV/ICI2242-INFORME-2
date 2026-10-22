project(
	'choosing_balls', 'cpp',
	version: '1.0.0',
	default_options: [
		'cpp_std=c++20',
		'warning_level=3',
		'buildtype=release',
	],
)

sources = files(
	'src/benchmark.cpp',
	'src/experiments.cpp',
	'src/generator.cpp',
	'src/solver.cpp',
	'src/main.cpp',
)

executable(
	'choosing_balls',
	sources,
	include_directories: include_directories('src'),
)
