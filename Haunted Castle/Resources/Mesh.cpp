#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Const.hpp"
#include "Mesh.hpp"

using namespace cgue;
using namespace std;


Mesh::Mesh()
{

}

Mesh::Mesh(string modelDir, char* nameMesh, aiMesh* mesh, const aiMaterial* material, glm::mat4x4 initTrans, glm::mat4x4 transform, Shader* shader)
{
	this->shader = shader;
	std::string str(nameMesh);
	this->nameMesh = str;
	this->meshTrans = initTrans * transform;
	this->updateTrans = mat4x4(1.0f);


	//cout << this->nameMesh << endl;

	/*
	applyUpdateTrans = false;
	char* tuerMitte = "TuerMitte";
	if (strcmp(nameMesh, tuerMitte) == 0)
	{
		applyUpdateTrans = true;
	}
	*/

	//cout << "Mesh: " << nameMesh << "   " << applyUpdateTrans << endl;
	//cout << "Mesh: "<< nameMesh << "   " << endl;
	//printMatGeometry(meshTrans);
	//name = mesh->mName.data;

	int texIndex = 0;
	aiString path;

	aiColor3D diffuseColor(0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
	diffuse = vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);

	aiColor3D specularColor(0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
	specular = vec3(specularColor.r, specularColor.g, specularColor.b);



	hasTexture = false;
	if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS)
	{
		string sTextureName = path.data;
		//cout << "sTextureName: " << sTextureName << endl;

		texture = new Texture(modelDir, sTextureName);

		hasTexture = true;
	}

	size = mesh->mNumFaces * 3;
	positions = new float[size * 3];
	indices = new int[size];
	normals = new float[size * 3];
	uvs = new float[size * 2];

	int iVertices = 0;
	for (int j = 0; j < mesh->mNumFaces; j++)
	{
		const aiFace& face = mesh->mFaces[j];
		for (int k = 0; k < face.mNumIndices; k++)
		{

			aiVector3D aiPos = mesh->mVertices[face.mIndices[k]];
			vec4 pos = vec4(aiPos.x, aiPos.y, aiPos.z, 1);/*initTrans * transform * */

			if (mesh->HasTextureCoords(k))
			{
				aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[0]];
				uvs[iVertices * 2 + 0] = uv.x;
				uvs[iVertices * 2 + 1] = uv.y;

				aiVector3D uv2 = mesh->mTextureCoords[0][face.mIndices[1]];
				uvs[iVertices * 2 + 2] = uv2.x;
				uvs[iVertices * 2 + 3] = uv2.y;

				aiVector3D uv3 = mesh->mTextureCoords[0][face.mIndices[2]];
				uvs[iVertices * 2 + 4] = uv3.x;
				uvs[iVertices * 2 + 5] = uv3.y;

			}

			aiVector3D aiNormal = mesh->HasNormals() ?
				mesh->mNormals[face.mIndices[k]] :
				aiVector3D(1.0f, 1.0f, 1.0f);
			vec4 normal = vec4(aiNormal.x, aiNormal.y, aiNormal.z, 1);/*initTrans * transform **/

			positions[iVertices * 3 + 0] = pos.x;
			positions[iVertices * 3 + 1] = pos.y;
			positions[iVertices * 3 + 2] = pos.z;

			if (xmin > pos.x) {
				xmin = pos.x;
			}
			if (xmax < pos.x) {
				xmax = pos.x;
			}
			if (ymin > pos.y) {
				ymin = pos.y;
			}
			if (ymax < pos.y) {
				ymax = pos.y;
			}
			if (zmin > pos.z) {
				zmin = pos.z;
			}
			if (zmax < pos.z) {
				zmax = pos.z;
			}

			indices[iVertices] = iVertices;

			normals[iVertices * 3 + 0] = normal.x;
			normals[iVertices * 3 + 1] = normal.y;
			normals[iVertices * 3 + 2] = normal.z;

			iVertices++;
		}
	}

	

	// face 1
	positionOfExtremValues[0] = vec3(xmin, ymin, zmin);
	positionOfExtremValues[1] = vec3(xmin, ymax, zmin);
	positionOfExtremValues[2] = vec3(xmax, ymin, zmin);
	positionOfExtremValues[3] = vec3(xmax, ymax, zmin);
	// face 2
	positionOfExtremValues[4] = vec3(xmin, ymin, zmax);
	positionOfExtremValues[5] = vec3(xmin, ymax, zmax);
	positionOfExtremValues[6] = vec3(xmax, ymin, zmax);
	positionOfExtremValues[7] = vec3(xmax, ymax, zmax);

	box.setVertex(positionOfExtremValues);

	sphereCenter = vec3((xmax - xmin) / 2, (ymax - ymin) / 2, (zmax - zmin) / 2);
	sphereRadius = length(vec3(xmax, ymax, zmax) - sphereCenter);

	/*
	// center of faces
	positionOfExtremValues[8] = vec4(xmin, 0, 0, 1);
	positionOfExtremValues[9] = vec4(xmax, 0, 0, 1);
	positionOfExtremValues[10] = vec4(0, ymin, 0, 1);
	positionOfExtremValues[11] = vec4(0, ymax, 0, 1);
	positionOfExtremValues[12] = vec4(0, 0, zmin, 1);
	positionOfExtremValues[13] = vec4(0, 0, zmax, 1);

	// edges
	positionOfExtremValues[14] = vec4(xmin, 0, zmin, 1);
	positionOfExtremValues[15] = vec4(xmin, 0, zmax, 1);
	positionOfExtremValues[16] = vec4(xmax, 0, zmin, 1);
	positionOfExtremValues[17] = vec4(xmax, 0, zmax, 1);
	positionOfExtremValues[18] = vec4(0, ymin, zmin, 1);
	positionOfExtremValues[19] = vec4(0, ymin, zmax, 1);
	positionOfExtremValues[20] = vec4(0, ymax, zmin, 1);
	positionOfExtremValues[21] = vec4(0, ymax, zmax, 1);
	*/


	//cout << "position: x:" << xmin << ", " << xmax << ", y " << ymin << ", " << ymax <<  ", z " << zmin << ", " << zmax << endl;


	// Load into Graphics Card

	// Load Data to Buffer
	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, size * 3 * sizeof(float),
		positions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int),
		indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// TUT 4 part 3 min 3
	glGenBuffers(1, &normalsBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normalsBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		size * 3 * sizeof(float), normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// UV Buffer
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * 2 * sizeof(float),
		uvs, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// Bind to Shader

	// Generate bindings
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	GLint positionIndex = glGetAttribLocation(shader->programHandle, "vertexPosition_modelspace");
	// FUER SAUBER PROGRAMMIEREN MUESSTE HIER NOCH UEBERPRUEFT WERDEN OB EH NICHT -1 ZURUECKKOMMT  
	glEnableVertexAttribArray(positionIndex);
	glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// TUT 4 part 3 min 3
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	GLint normalIndex = glGetAttribLocation(shader->programHandle, "vertexNormal_modelspace");
	glEnableVertexAttribArray(normalIndex);
	glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	GLint uvIndex = glGetAttribLocation(shader->programHandle, "vertexUV");
	glEnableVertexAttribArray(uvIndex);
	glVertexAttribPointer(uvIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	// TUT 4 part 3 min 3

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &normalsBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteVertexArrays(1, &vao);

	delete positions; positions = nullptr;
	delete normals; normals = nullptr;
	delete indices; indices = nullptr;
	delete uvs; uvs = nullptr;
	delete texture; texture = nullptr;
}

void Mesh::loadUniforms(Shader* shader, mat4x4 view, mat4x4 proj, mat4x4 globalPose)
{
	mat4x4 model = globalPose * meshTrans * updateTrans;
	mat4x4 mv = view * model;
	mat4x4 mvp = proj * mv;
	vec3 LightPosition_worldspace = lightPos;

	// Model
	auto model_location = glGetUniformLocation(shader->programHandle, "M");
	glUniformMatrix4fv(model_location, 1, GL_FALSE, value_ptr(model));

	// View
	auto view_location = glGetUniformLocation(shader->programHandle, "V");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, value_ptr(view));

	// Model View
	auto model_view_location = glGetUniformLocation(shader->programHandle, "MV");
	glUniformMatrix4fv(model_view_location, 1, GL_FALSE, value_ptr(mv));

	// Model View Proj
	auto model_view_proj_location = glGetUniformLocation(shader->programHandle, "MVP");
	glUniformMatrix4fv(model_view_proj_location, 1, GL_FALSE, value_ptr(mvp));

	GLuint depthMatrixID = glGetUniformLocation(shader->programHandle, "depthMVP");
	glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &mvp[0][0]);


	// Light
	auto LightPosition_worldspace_location = glGetUniformLocation(shader->programHandle, "LightPosition_worldspace");
	glUniform3f(LightPosition_worldspace_location, LightPosition_worldspace.x, LightPosition_worldspace.y, LightPosition_worldspace.z);

	// Torch1
	auto Torch1Position_worldspace_location = glGetUniformLocation(shader->programHandle, "Torch1Position_worldspace");
	glUniform3f(Torch1Position_worldspace_location, torch1Pos.x, torch1Pos.y, torch1Pos.z);

	// Torch2
	auto Torch2Position_worldspace_location = glGetUniformLocation(shader->programHandle, "Torch2Position_worldspace");
	glUniform3f(Torch2Position_worldspace_location, torch2Pos.x, torch2Pos.y, torch2Pos.z);

	// Diffuse
	auto diffuseColor = glGetUniformLocation(shader->programHandle, "diffuseColor");
	glUniform3f(diffuseColor, diffuse.r, diffuse.g, diffuse.b);

	// Specular
	auto specularColor = glGetUniformLocation(shader->programHandle, "specularColor");
	glUniform3f(specularColor, specular.r, specular.g, specular.b);

	// Texture
	auto tex_enabled = glGetUniformLocation(shader->programHandle, "hasTexture");
	if (hasTexture) {
		glUniform1i(tex_enabled, 1);

		texture->bind(3);

		auto myTextureSampler_location = glGetUniformLocation(shader->programHandle, "myTextureSampler");
		glUniform1i(myTextureSampler_location, 3);
	}
	else {
		glUniform1i(tex_enabled, 0);
	}
}

void Mesh::setMeshTrans(mat4x4 trans)
{
	this->meshTrans = trans;
}

void Mesh::draw(Shader* shader, mat4x4 view, mat4x4 proj, mat4x4 globalPose, bool cull)
{ 
	if (!VIEWFRUSTUM_CULLING || !cull || isVisible(globalPose * meshTrans * updateTrans)){
		loadUniforms(shader, view, proj, globalPose);

		glBindVertexArray(vao);
		glDrawElements(CGUE_RENDER, size, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	else {
		NUMBER_OF_CULLED_MESHES++;
	}
}

void Mesh::update(float time_delta, float time_abs)
{
	/*
	cout << nameMesh << endl;
	cout << time_abs << endl;
	cout << updateTrans[3][1] << endl;
	*/

	if ((nameMesh.compare("polySurface105") == 0 || 
		nameMesh.compare("leglow1_group_leglow1_pasted__polySurface74_leglow1_polySurface") == 0) && 
		time_abs > 10 && updateTrans[3][1] > -2)
	{
		updateTrans = translate(this->updateTrans, vec3(0, -0.01, 0));
	}

	/*
	char* tuerMitte = "TuerMitte";
	if (strcmp(nameMesh, tuerMitte) == 0)
	{

	}
	*/
	/*
	if (applyUpdateTrans)
	{
		//cout << nameMesh << endl;
		this->updateTrans = rotate(this->updateTrans, 10.0f * time_delta, vec3(0, 0, 1));
	}
	*/
}

void Mesh::setPhysX(PxPhysics* mPhysicsSDK, PxFoundation* mFoundation, PxDefaultErrorCallback mDefaultErrorCallback, PxDefaultAllocator mDefaultAllocatorCallback)
{
	this->gPhysicsSDK = mPhysicsSDK;
	this->gFoundation = mFoundation;
	this->gDefaultErrorCallback = mDefaultErrorCallback;
	this->gDefaultAllocatorCallback = mDefaultAllocatorCallback;
}

// checks if any part of an object is visble 
// if not returns false else returns true
bool Mesh::isVisible(mat4x4 globalPose){
	return frustumG->boxInFrustum(box, globalPose) != 0;
}