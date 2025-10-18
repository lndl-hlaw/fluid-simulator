Project during development.

Consists of two parts:
- Lagrangian simulator of fluid. Written in `cuda` on GPU.
- Visualizator in `OpenGL`.

Simulator uses [SPH](https://sph-tutorial.physics-simulation.org/pdf/SPH_Tutorial.pdf) for the pressure model.

#### Current project status

A particle simulation is basically implemented. Allows to aply forces etc. Current version out of the box runs with gravity and pressure applied.

![current progress visual](./imgs/simulator1.png)

Visualization is implemented with `cuda`-`OpenGL` interop.

#### Bibliography

- https://sph-tutorial.physics-simulation.org/pdf/SPH_Tutorial.pdf
- https://web.archive.org/web/20250106201614/http://www.ligum.umontreal.ca/Clavet-2005-PVFS/pvfs.pdf
- https://matthias-research.github.io/pages/publications/sca03.pdf
- https://www.youtube.com/watch?v=rSKMYc1CQHE

