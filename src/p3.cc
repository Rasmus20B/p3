module;

#include <cmath>
#include <print>
#include <mdspan>

#include "raylib.h"

import jcast;
import ecstasy;

import p3.components;
import p3.types;

template<size_t N, typename... C>
using Scene = ecstasy::EntityManager<N, C...>;

export module p3;

namespace p3 {

  Mesh rmesh_from_jmesh(jcast::Mesh m1) {
    Mesh real = {0};
    real.vertexCount = m1.vertices.size() * 3;
    real.triangleCount = m1.vertices.size();

    real.vertices = (float*)MemAlloc(real.vertexCount * 3 * sizeof(float));
    real.texcoords = (float*)MemAlloc(real.vertexCount * 2 * sizeof(float));
    real.normals = (float*)MemAlloc(real.vertexCount * 3 * sizeof(float));

    int i = 0;
    int j = 0;
    int k = 0;
    for(auto v: m1.vertices) {
      real.vertices[i++] = v.position.x;
      real.vertices[i++] = v.position.y;
      real.vertices[i++] = v.position.z;
      real.normals[j++] = v.normal.x;
      real.normals[j++] = v.normal.y;
      real.normals[j++] = v.normal.z;
      real.texcoords[k++] = v.texcoords.y;
      real.texcoords[k++] = v.texcoords.x;
    }

    return real;
  }

  CMesh get_mesh_from_jcast(jcast::Mesh jm) {

    CMesh m;
    m.vertices.resize(jm.vertices.size());
    m.normals.resize(jm.vertices.size());
    m.texcoords.resize(jm.vertices.size());

    for(int i = 0; i < m.vertices.size(); ++i) {
      m.vertices[i].x = jm.vertices[i].position.x;
      m.vertices[i].y = jm.vertices[i].position.y;
      m.vertices[i].z = jm.vertices[i].position.z;

      m.normals[i].x = jm.vertices[i].normal.x;
      m.normals[i].y = jm.vertices[i].normal.y;
      m.normals[i].z = jm.vertices[i].normal.z;

      m.texcoords[i].x = jm.vertices[i].texcoords.x;
      m.texcoords[i].y = jm.vertices[i].texcoords.y;
    }
    return m;
  }

  Mesh rmesh_from_cmesh(CMesh mesh_component) {
    Mesh m = {0};

    m.vertexCount = mesh_component.vertices.size() * 3;
    m.triangleCount = mesh_component.vertices.size();
    m.vertices = (f32*)MemAlloc(m.vertexCount * 3 * sizeof(f32));
    m.texcoords = (f32*)MemAlloc(m.vertexCount * 2 * sizeof(f32));
    m.normals = (f32*)MemAlloc(m.vertexCount * 3 * sizeof(f32));

    auto v_span = std::mdspan(m.vertices, m.vertexCount, 3);
    auto n_span = std::mdspan(m.normals, m.vertexCount, 3);
    auto t_span = std::mdspan(m.texcoords, m.vertexCount, 2);
    for(int i = 0; i < v_span.extent(0); ++i) {
      v_span[i, 0] = mesh_component.vertices[i].x;
      v_span[i, 1] = mesh_component.vertices[i].y;
      v_span[i, 2] = mesh_component.vertices[i].z;

      n_span[i, 0] = mesh_component.normals[i].x;
      n_span[i, 1] = mesh_component.normals[i].y;
      n_span[i, 2] = mesh_component.normals[i].z;

      t_span[i, 0] = mesh_component.texcoords[i].x;
      t_span[i, 1] = mesh_component.texcoords[i].y;
    }
    return m;
  }

  template<size_t N, typename ...C>
  void back_and_forth(Scene<N, C...>& s) {
    auto v_ents = s.template get_associated_entities<CVelocity3D>();
    for(auto v: v_ents) {
      auto& pos = ecstasy::getComponent<CTransform3D>(s.cm, v);
      auto& vel = ecstasy::getComponent<CVelocity3D>(s.cm, v);

      if(pos.position.x >= 4.f) {
        vel.v.x = -0.002f;
      } else if(pos.position.x <= -4.f) {
        vel.v.x = 0.002f;
      }

      pos.position.x += vel.v.x;
      pos.rotation.x += 0.05;
    }
  }

  export void main_loop() {

    InitWindow(1920, 1080, "P3 Window");
    Scene<1000, CTransform3D, CVelocity3D, CMesh, CModel> scene;
    auto player = scene.create_entity();
    auto cube = jcast::load_mesh("../assets/monkey.dae");
    auto m = Mesh{rmesh_from_jmesh(cube)};

    UploadMesh(&m, true);

    Model model = LoadModelFromMesh(m);
    Model check = LoadModelFromMesh(GenMeshCube(2.0f, 1.0, 2.f));

    scene.add_components<CTransform3D, CVelocity3D, CMesh, CModel>(player, 
        CTransform3D{
          .position {
            .x = 0.f,
            .y = 1.f,
            .z = 0.f
          },
          .scale {
            .x = 1.f,
            .y = 1.f,
            .z = 1.f
          }
        },
        CVelocity3D{
          .v {
            .x = 0.002f
          }
        },
        CMesh{},
        CModel{model}
        );

    auto box = scene.create_entity();
    scene.add_components<CTransform3D, CMesh, CModel>(box, 
        CTransform3D{
          .position {
            .x = 4,
            .y = 3,
            .z = 0
          },
          .scale {
            .x = 1.f,
            .y = 1.f,
            .z = 1.f
          }
        },
        CMesh{},
        CModel{check}
        );

     
  Camera camera = { { 0.0f, 1.0f, 5.0f }, { 0.0f, 2.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 60.0f, CAMERA_PERSPECTIVE };
    while(!WindowShouldClose()) {
      UpdateCameraPro(&camera, {
        (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) * 0.001f -
        (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))*0.001f,    
        (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))*0.001f -   // Move right-left
        (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))*0.001f,
        0.0f 
        },
        (Vector3){
                GetMouseDelta().x*0.1f,                            // Rotation: yaw
                GetMouseDelta().y*0.1f,                            // Rotation: pitch
                0.0f                                                // Rotation: roll
            },
        GetMouseWheelMove() * 2.0f
        );

      back_and_forth(scene);
      BeginDrawing();
      ClearBackground(RAYWHITE);

      BeginMode3D(camera);

      DrawGrid(10, 1.f);

      auto m_ents = scene.get_associated_entities<CModel>();
      for(auto m: m_ents) {
        auto tr = ecstasy::getComponent<CTransform3D>(scene.cm, m);
        DrawModelEx(ecstasy::getComponent<CModel>(scene.cm, m).m, 
                  tr.position, 
                  {1.f, 0, 0},
                  tr.rotation.x,
                  tr.scale,
                  DARKBLUE);
      }
      EndMode3D();
      EndDrawing();
    }

    CloseWindow();
  }

}
