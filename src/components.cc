module;

#include <vector>
#include "raylib.h"

import p3.types;

export module p3.components;

export namespace p3 {
  struct CTransform3D {
    vec3 position;
    vec3 scale;
    vec3 rotation;
  };

  struct CVelocity3D {
    vec3 v;
  };

  struct CMesh {
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<vec2> texcoords;
  };

  struct CModel {
    Model m;
  };
}
