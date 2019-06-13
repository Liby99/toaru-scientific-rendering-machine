#include <Python/Python.h>
#include <iostream>

#include <toaru/toaru.h>

using namespace toaru;

static PyObject *mpm_simulate(PyObject *self, PyObject *args) {

  // Variables
  int frameCount = 2, particleCount = 30;

  // Initialize grid and initial particles
  mpm::Grid grid(Vector3f(0, 0, 0), Vector3f(1, 1, 1), Vector3u(100, 100, 100));
  for (int i = 0; i < particleCount; i++) {
    grid.addParticle(mpm::Particle(Vector3f(
      Math::random(-0.5f, 0.5f),
      Math::random(-0.5f, 0.5f),
      Math::random(-0.5f, 0.5f)
    )));
  }

  // Generate the keyframes
  auto keyframes = PyList_New(frameCount);
  for (int i = 0; i < frameCount; i++) {

    // Fill in the data
    auto vertices = PyList_New(grid.particles.size());
    for (int j = 0; j < grid.particles.size(); j++) {
      auto pos = PyTuple_New(3);
      for (int k = 0; k < 3; k++) {
        PyTuple_SetItem(pos, k, PyFloat_FromDouble(grid.particles[j].position(k)));
      }
      PyList_SetItem(vertices, j, pos);
    }

    // Set the keyframe
    PyList_SetItem(keyframes, i, vertices);

    // Step forward
    grid.step();
  }

  // Return the generated keyframes
  return keyframes;
}

static PyMethodDef MpmMethods[] = {
  {"simulate", mpm_simulate, METH_VARARGS, "Do mpm simulation and return an array of keyframes of point cloud"},
  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initpympm(void) {
  (void) Py_InitModule("pympm", MpmMethods);
}