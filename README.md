# BrotRender
Software for offline CPU rendering of the Mandelbrot set and Buddhabrot-style fractals.

As it stands, the code is in a state to get a result as fast as possible, and is not very presentable. This is also why the regular Mandelbrot sets are rendered on the CPU rather than the GPU.

## TODOs
- Object-orient-ize and refactor
- Don't hard-code to write files to *my* desktop
- Implement 4x4 matrix, proper rotations, proper projections
- Apply unique coloring method to Newton fractals
- Implement framework for rendering animations
- Experiment with OpenGL and CUDA for rendering regular Mandelbrot sets
