#ifndef MODEL_H
#define MODEL_H

#include<iostream>
#include<fstream>
#include<map>
#include<string>
#include<Importer.hpp>
#include<postprocess.h>
#include<scene.h>
#include<vector>
#include"Texture.h"
#include"utils.h"

class Model
{
private:
	GLfloat xrot;
	GLfloat yrot;
	GLfloat zrot;
	
	Utils utils;
	Texture texture;
	Assimp::Importer importer;
	const aiScene* scene = NULL;
	GLuint scene_list = 0;
	
	bool LoadTextures(const aiScene* scene);
	void apply_mat(const aiMaterial *mat);	
	void recursive_render(const struct aiScene *sc, const struct aiNode* nd, float scale);
	void Model::renderScene(const aiScene* scene, float scale);
public:

	void Draw(GLfloat translateX, GLfloat translateY, GLfloat translateZ, GLfloat angle, GLfloat x, GLfloat y, GLfloat z, GLfloat scale);
	bool LoadModel(const std::string& filename);


};

#endif