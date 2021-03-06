# PROJECT: RAY TRACER

In this project, we will implement a basic ray tracer.

### Installing

1. Clone this repository to a directory of your choice.
2. Use 'make' command to compile both of the executables: ray_tracer, ray_tracer_preview
If opengl is not available, use 'make ray_tracer' to compile only the version without opengl viewer.

## Running the tests

Two different version of the program is available:
  ray_tracer: This is the program that the grading script uses, this generates a output.png and diff.png files if the solution file is available
  ray_tracer_preview: Same as ray_tracer, but it displays the rendered image (and the solution next to it, if available) in an opengl context.

  To run a test:
    ./ray_tracer -i <test_filename>  #OR
    ./ray_tracer_preview -i <test_filename>
  e.g.
    ./ray_tracer_preview -i ./tests/00.txt

## Authors

* **Zhen Bian** - *Initial work* - [Basic Ray Tracer](https://github.com/zbian002/Basic-Ray-Tracer)

