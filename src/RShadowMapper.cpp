#include"RShadowMapper.h"


void RShadowMapper::init(){
  glGenFramebuffers(1, &fb);
  glBindFramebuffer(GL_FRAMEBUFFER, fb);

  glGenTextures(1, &stex);
  glBindTexture(GL_TEXTURE_2D, stex);
  /*
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWMAP_X, SHADOWMAP_Y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 


  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
  */
  glTexStorage2D(GL_TEXTURE_2D, 11, GL_DEPTH_COMPONENT32F,  SHADOWMAP_X, SHADOWMAP_Y);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glBindTexture(GL_TEXTURE_2D, 0);


  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, stex, 0);
  // glDrawBuffer(GL_NONE); // No color buffer is drawn to

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  RShader vs, fs;
  const char *VS_SOURCE = GLSL(330,
			       layout (location = 0) in vec3 in_vertex; //vertex position
			       layout (location = 1) in vec3 pos; //Instance position
			       layout (location = 2) in vec3 color; //Instanced color 
			       layout (location = 3) in float scale; //Instanced scale
			       uniform mat4 depthMVP;
			       
			       void main () {
				 gl_Position = depthMVP*(vec4(scale*in_vertex.xyz+pos.xyz,1.0));
			       }
			       
			       );
  const char *FS_SOURCE = GLSL(330,
			       layout(location = 0) out float fragmentdepth;
			       void main(){
				 fragmentdepth = (gl_FragCoord.z);
			       }
			       );

  vs.charload(VS_SOURCE, GL_VERTEX_SHADER);
  fs.charload(FS_SOURCE, GL_FRAGMENT_SHADER);
  pr.initialize(vs, fs);
  pr.use();

  glm::vec3 lightInvDir = glm::vec3(10,10,10);

  // Compute the MVP matrix from the light's point of view
  glm::mat4 depthProjectionMatrix = glm::ortho<float>(-1,1,-1,1,0.1,1000);//
  glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0), glm::vec3(0,1,0));
  //glm::mat4 depthModelMatrix = glm::mat4(1.0);
  depthP = depthProjectionMatrix;
  depthV =  depthViewMatrix ;
  /*
  depthMVP = glm::mat4(glm::vec4(1.79259,0,0,0),
		       glm::vec4(0,-1.26755,-0.707108,-0.707107),
		       glm::vec4(0,-1.26755,0.707108,0.707107),
		       glm::vec4(0,0,14.1222,14.1421));
*/
  glm::mat4 biasMatrix(
		       0.5, 0.0, 0.0, 0.0,
		       0.0, 0.5, 0.0, 0.0,
		       0.0, 0.0, 0.5, 0.0,
		       0.5, 0.5, 0.5, 1.0
		       );

  depthBiasPV = biasMatrix*depthP*depthV;

  depthMVPid = pr.get_attrib_handle("depthMVP");
  
  glm::mat4 depthPV = depthP*depthV;
  glUniformMatrix4fv(depthMVPid, 1, GL_FALSE, &depthPV[0][0]);
  pr.unbind();
  this->enabled = true;
}


void RShadowMapper::prepare_to_draw(glm::mat4& MVP){
  glBindFramebuffer(GL_FRAMEBUFFER, fb);
  glViewport(0,0,SHADOWMAP_X,SHADOWMAP_Y);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_DEPTH_BUFFER_BIT);
  pr.use();
  model_matrix = MVP;
  //depthMVP = depthMV*model;
  glUniformMatrix4fv(depthMVPid, 1, GL_FALSE, &MVP[0][0]);

}

void RShadowMapper::flush(){

  pr.unbind();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RShadowMapper::attach_shadowmap(GLuint prr){

  glm::mat4 biasMatrix(
		       0.5, 0.0, 0.0, 0.0,
		       0.0, 0.5, 0.0, 0.0,
		       0.0, 0.0, 0.5, 0.0,
		       0.5, 0.5, 0.5, 1.0
		       );
  
  glm::mat4 depthBiasMVP = biasMatrix*depthP*depthV*model_matrix;
  glUniformMatrix4fv(glGetUniformLocation(prr,"depthBiasMVP"), 1, GL_FALSE, &depthBiasMVP[0][0]);
    glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, stex);
  //  glUniform1i(glGetUniformLocation(prr,"shadowmap"), 0);

}
