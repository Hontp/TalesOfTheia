#include "Model.h"

bool Model::LoadModel(const std::string& filename)
{
	std::fstream fin(filename.c_str());
	if (!fin.fail())
	{
		fin.close();
	}
	else
	{
		std::cout << "Can not open file " << filename.c_str() << "\n";
		return false;
	}

	scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality);

	if (!scene)
	{
		std::cout << "Error: " << importer.GetErrorString() << "\n";
		return false;
	}

	return true;
}

void Model::apply_mat(const aiMaterial *mat)
{
	float c[4];
	GLenum fill_mode;
	int ret1, ret2;
	unsigned int texId = 0;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;	// changed: to unsigned

	int texIndex = 0;
	aiString texPath;	//contains filename of texture

	glPushMatrix();
	if (AI_SUCCESS == mat->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
	{
		//bind texture	
		texId = texture.LoadTexture(texPath.C_Str(), true);
		glBindTexture(GL_TEXTURE_2D, texId);
	}
	
	utils.set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		utils.color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	utils.set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specular))
		utils.color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	utils.set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &ambient))
		utils.color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	utils.set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_EMISSIVE, &emission))
		utils.color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS, &shininess, &max);
	max = 1;
	ret2 = aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
	if ((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		utils.set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if (AI_SUCCESS == aiGetMaterialIntegerArray(mat, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	// cull geometery
	/*
	max = 1;
	if ((AI_SUCCESS == aiGetMaterialIntegerArray(mat, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);*/

	glPopMatrix();
}

void Model::recursive_render(const struct aiScene *sc, const struct aiNode* nd, float scale)
{
	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = nd->mTransformation;

	m.Scaling(aiVector3D(scale, scale, scale), m);

	// update transform
	m.Transpose();
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		apply_mat(sc->mMaterials[mesh->mMaterialIndex]);


		if (mesh->mNormals == NULL)
		{
			glDisable(GL_LIGHTING);
		}
		else
		{
			glEnable(GL_LIGHTING);
		}

		if (mesh->mColors[0] != NULL)
		{
			glEnable(GL_COLOR_MATERIAL);
		}
		else
		{
			glDisable(GL_COLOR_MATERIAL);
		}



		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch (face->mNumIndices)
			{
			case 1: face_mode = GL_POINTS; break;
			case 2: face_mode = GL_LINES; break;
			case 3: face_mode = GL_TRIANGLES; break;
			default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for (i = 0; i < face->mNumIndices; i++)		// go through all vertices in face
			{
				int vertexIndex = face->mIndices[i];	// get group index for current index
				if (mesh->mColors[0] != NULL)
					utils.Color4f(&mesh->mColors[0][vertexIndex]);
				if (mesh->mNormals != NULL)

					if (mesh->HasTextureCoords(0))		//HasTextureCoords(texture_coordinates_set)
					{
					glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, 1 - mesh->mTextureCoords[0][vertexIndex].y); //mTextureCoords[channel][vertex]
					}

				glNormal3fv(&mesh->mNormals[vertexIndex].x);
				glVertex3fv(&mesh->mVertices[vertexIndex].x);
			}

			glEnd();

		}

	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		recursive_render(sc, nd->mChildren[n], scale);
	}

	glPopMatrix();
}

void Model::renderScene(const aiScene* scene, float scale)
{

	recursive_render(scene, scene->mRootNode, scale);

}

void Model::Draw(GLfloat translateX, GLfloat translateY, GLfloat translateZ, 
				GLfloat angle, GLfloat x ,GLfloat y, GLfloat z, GLfloat scale)				
{
	glPushMatrix();
	glLoadIdentity();				// Reset MV Matrix

	glTranslatef(translateX, translateY, translateZ);	// Move object arond the screen

	// rotate object in x, y, z positions
	glRotatef(angle, x, y, z);

	// draw object to screen
	renderScene(scene, scale);
	// clear texture from memory
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

}
