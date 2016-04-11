#include "RGLHandle.h"

RGLHandle::RGLHandle(){}
RGLHandle::~RGLHandle(){}
                                                                                               

bool RGLHandle::init(int maxN){
  printf("Initializing OpenGL...\n");
  this->handle_resize();

  this->maxN = maxN;
  attribs["in_vertex"] = 0;
  attribs["pos"] = 1;
  attribs["color"] = 2;
  attribs["scale"] = 3;

  init_buffers();
  init_math();
  init_shaders();
  init_uniforms();
  printf("DONE!\n");
  return true;
}

bool RGLHandle::init_buffers(){
  printf("\tInit buffers...     ");
  init_sphere();
  init_instance_vbos();
  init_vao();
  printf("DONE!\n");
  return true;
}
bool RGLHandle::init_sphere(){ //Config and upload sphere vertices
  DataLayout dl;
  dl.init(3, 3*sizeof(float), GL_FLOAT, 0);
  sphere_vbos[0].init(GL_ARRAY_BUFFER, GL_MAP_READ_BIT, dl);
  sphere_vbos[1].init(GL_ELEMENT_ARRAY_BUFFER, GL_MAP_READ_BIT);
  fill_sphere_vbos(sphere_vbos[0], sphere_vbos[1]);

  return true;
}

bool RGLHandle::init_vao(){ //Configure Vertex Array Object
  spheres_vao.set_attrib(attribs["in_vertex"], sphere_vbos[0], 0);
  spheres_vao.set_attrib(attribs["pos"], instances_vbos[0], 1);  
  spheres_vao.set_attrib(attribs["color"], instances_vbos[1], 2);
  spheres_vao.set_attrib(attribs["scale"], instances_vbos[2], 3);

  return true;
}

bool RGLHandle::init_instance_vbos(){
  DataLayout dl;
  dl.init(3, 3*sizeof(float), GL_FLOAT, 0, 1);
  instances_vbos[0].init(GL_ARRAY_BUFFER, GL_DYNAMIC_STORAGE_BIT, dl);
  instances_vbos[1].init(GL_ARRAY_BUFFER, GL_DYNAMIC_STORAGE_BIT, dl);
  dl.init(1, sizeof(float), GL_FLOAT, 0, 1);
  instances_vbos[2].init(GL_ARRAY_BUFFER, GL_DYNAMIC_STORAGE_BIT, dl);
  //Create buffer with max possible N
  instances_vbos[0].initmem(maxN*sizeof(float)*3, NULL);  //poss
  instances_vbos[1].initmem(maxN*sizeof(float)*3, NULL);  //colors
  instances_vbos[2].initmem(maxN*sizeof(float)*1, NULL);  //scales
  return true;
}


bool RGLHandle::init_math(){
  proj =glm::perspective(45.0f, FWIDTH/(float)FHEIGHT, 0.01f, 10000.0f);
  model = glm::mat4();
  view = cam.lookAt();
  rotate_model(M_PI/4.0f, 1.0f, 0.0f, 0.0f);

  MVP = proj*view*model;

  return true;
}

#include "shaders.h"
bool RGLHandle::init_shaders(){  
  printf("\tInit shaders...     ");
  RShader shs[2];
  shs[0].charload(VS_SOURCE, GL_VERTEX_SHADER);
  shs[1].charload(FS_SOURCE, GL_FRAGMENT_SHADER);
  pr.init(shs, 2);
  printf("DONE!\n");
  
  return true;
}
bool RGLHandle::init_uniforms(){
  printf("\tInit uniforms...    ");
  pr.use();
  uniMVP = glGetUniformLocation(pr.id(), "MVP");
  unimodel = glGetUniformLocation(pr.id(), "model");
  pr.unbind();
  printf("DONE!\n");
  return true;
}


bool RGLHandle::upload_instances(const float *pos, const float *colors, const float *scales, int N){  
  instances_vbos[0].upload(0, 3*N*sizeof(float), (const void *)pos);
  instances_vbos[1].upload(0, 3*N*sizeof(float), (const void *)colors);
  instances_vbos[2].upload(0, 1*N*sizeof(float), (const void *)scales);
  currentN = N;
  return true;
}

void RGLHandle::handle_resize(){
  glViewport(0,0,FWIDTH, FHEIGHT);
  float ar = FWIDTH/(float)FHEIGHT;
  proj =glm::perspective(45.0f, ar, 0.01f, 10000.0f);
}

void RGLHandle::rotate_model(GLfloat angle, GLfloat x, GLfloat y, GLfloat z){
  model = glm::rotate(model, angle,  glm::vec3(x, y, z));
}

void RGLHandle::update(){
  cam.update();
}

void RGLHandle::draw(){
  pr.use();
  view = cam.lookAt();
  MVP = proj*view*model;
  glUniformMatrix4fv(uniMVP , 1, GL_FALSE, glm::value_ptr(MVP));
  glUniformMatrix4fv(unimodel , 1, GL_FALSE, glm::value_ptr(model));
  glUniform3f(glGetUniformLocation(pr.id(), "EyeWorldPos"), cam.pos.x, cam.pos.y, cam.pos.z);
  spheres_vao.use();
  sphere_vbos[1].use(); //indices
  glDrawElementsInstanced(GL_TRIANGLES, 240, GL_UNSIGNED_INT, NULL, currentN); 
  sphere_vbos[1].unbind();
  spheres_vao.unbind();
  pr.unbind();
}






void fill_sphere_vbos(VBO &posVBO, VBO &indicesVBO){

  float v[]={
    0.0000, -1.0000,  0.0000,  0.4253, -0.8507,  0.3090,
    -0.1625, -0.8507,  0.5000,  0.7236, -0.4472,  0.5257,
    0.8506, -0.5257,  0.0000, -0.5257, -0.8507,  0.0000,
    -0.1625, -0.8507, -0.5000,  0.4253, -0.8507, -0.3090,
    0.9511,  0.0000,  0.3090, -0.2764, -0.4472,  0.8506,
    0.2629, -0.5257,  0.8090,  0.0000,  0.0000,  1.0000,
    -0.8944, -0.4472,  0.0000, -0.6882, -0.5257,  0.5000,
    -0.9511,  0.0000,  0.3090, -0.2764, -0.4472, -0.8506,
    -0.6882, -0.5257, -0.5000, -0.5878,  0.0000, -0.8090,
    0.7236, -0.4472, -0.5257,  0.2629, -0.5257, -0.8090,
    0.5878,  0.0000, -0.8090,  0.5878,  0.0000,  0.8090,
    -0.5878,  0.0000,  0.8090, -0.9511,  0.0000, -0.3090,
    0.0000,  0.0000, -1.0000,  0.9511,  0.0000, -0.3090,
    0.2764,  0.4472,  0.8506,  0.6882,  0.5257,  0.5000,
    0.1625,  0.8507,  0.5000, -0.7236,  0.4472,  0.5257,
    -0.2629,  0.5257,  0.8090, -0.4253,  0.8507,  0.3090,
    -0.7236,  0.4472, -0.5257, -0.8506,  0.5257,  0.0000,
    -0.4253,  0.8507, -0.3090,  0.2764,  0.4472, -0.8506,
    -0.2629,  0.5257, -0.8090,  0.1625,  0.8507, -0.5000,
    0.8944,  0.4472,  0.0000,  0.6882,  0.5257, -0.5000,
    0.5257,  0.8507,  0.0000,  0.0000,  1.0000,  0.0000
  };
  GLuint index[]={
    0,  1,  2,  3,  1,  4,  0,  2,  5,  0, 
    5,  6,  0,  6,  7,  3,  4,  8,  9, 10, 
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
    3,  8, 21,  9, 11, 22, 12, 14, 23, 15, 
    17, 24, 18, 20, 25, 26, 27, 28, 29, 30, 
    31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 
    2, 10,  9,  2,  1, 10,  1,  3, 10,  4, 
    7, 18,  4,  1,  7,  1,  0,  7,  5, 13, 
    12,  5,  2, 13,  2,  9, 13,  6, 16, 15, 
    6,  5, 16,  5, 12, 16,  7, 19, 18,  7, 
    6, 19,  6, 15, 19,  8, 25, 38,  8,  4, 
    25,  4, 18, 25, 11, 21, 26, 11, 10, 21, 
    10,  3, 21, 14, 22, 29, 14, 13, 22, 13, 
    9, 22, 17, 23, 32, 17, 16, 23, 16, 12, 
    23, 20, 24, 35, 20, 19, 24, 19, 15, 24, 
    21, 27, 26, 21,  8, 27,  8, 38, 27, 22, 
    30, 29, 22, 11, 30, 11, 26, 30, 23, 33, 
    32, 23, 14, 33, 14, 29, 33, 24, 36, 35, 
    24, 17, 36, 17, 32, 36, 25, 39, 38, 25, 
    20, 39, 20, 35, 39, 28, 40, 41, 28, 27, 
    40, 27, 38, 40, 31, 28, 41, 31, 30, 28, 
    30, 26, 28, 34, 31, 41, 34, 33, 31, 33, 
    29, 31, 37, 34, 41, 37, 36, 34, 36, 32, 
    34, 40, 37, 41, 40, 39, 37, 39, 35, 37
  };
  posVBO.initmem(sizeof(v), v);
  indicesVBO.initmem(sizeof(index), index);
}
