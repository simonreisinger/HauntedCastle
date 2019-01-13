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
	this->updateTrans = mat4x4(1.0f);

	shader->useShader();

	iMeshesLoaded++;
	//cout << "Loading Mesh " << iMeshesLoaded << " of " << countMeshesLoading << endl;

	int texIndex = 0;
	aiString path;

	aiColor3D diffuseColor(0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
	diffuse = vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);

	aiColor3D specularColor(0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
	specular = vec3(specularColor.r, specularColor.g, specularColor.b);


	hasTexture = false;
	hasNormalTexture = false;

	for (int texIndex = 0; texIndex < material->GetTextureCount(aiTextureType_DIFFUSE); texIndex++)
	{
		if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS)
		{
			string sTextureName = path.data;
			//cout << "sTextureName: " << sTextureName << endl;

			texture = new Texture(modelDir, sTextureName);

			hasTexture = true;
		}
	}

	for (int texIndex = 0; texIndex < material->GetTextureCount(aiTextureType_NORMALS); texIndex++)
	{
		if (material->GetTexture(aiTextureType_NORMALS, texIndex, &path) == AI_SUCCESS)
		{
			string sTextureName = path.data;
			//cout << "sTextureName: " << sTextureName << endl;

			normalTexture = new Texture(modelDir, sTextureName);

			hasNormalTexture = true;
		}
	}

	size = mesh->mNumFaces * 3;
	positions = new float[size * 3];
	indices = new int[size];
	normals = new float[size * 3];
	tangents = new float[size * 3];
	bitangents = new float[size * 3];
	uvs = new float[size * 2];

	//cout << "Mesh " << nameMesh << ":" << endl;
	int iVertices = 0;
	for (int j = 0; j < mesh->mNumFaces; j++)
	{
		const aiFace& face = mesh->mFaces[j];

		//cout << "  Face " << j << ":" << endl;


		for (int k = 0; k < face.mNumIndices; k++)
		{
			//cout << "    Face Index " << k << ":" << endl;

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


			if (mesh->HasNormals())
			{
				aiVector3D aiNormal = mesh->HasNormals() ?
					mesh->mNormals[face.mIndices[k]] :
					aiVector3D(1.0f, 1.0f, 1.0f);
				vec4 normal = vec4(aiNormal.x, aiNormal.y, aiNormal.z, 1);/*initTrans * transform **/

				//cout << "      Normal: " << aiNormal.x << ", " << aiNormal.y << ", " << aiNormal.z << endl;

				normals[iVertices * 3 + 0] = normal.x;
				normals[iVertices * 3 + 1] = normal.y;
				normals[iVertices * 3 + 2] = normal.z;
			}


			if (mesh->HasTangentsAndBitangents())
			{
				aiVector3D tangent = mesh->mTangents[face.mIndices[k]];

				tangents[iVertices * 3 + 0] = tangent.x;
				tangents[iVertices * 3 + 1] = tangent.y;
				tangents[iVertices * 3 + 2] = tangent.z;

				aiVector3D bitangent = mesh->mBitangents[face.mIndices[k]];

				bitangents[iVertices * 3 + 0] = bitangent.x;
				bitangents[iVertices * 3 + 1] = bitangent.y;
				bitangents[iVertices * 3 + 2] = bitangent.z;

				//cout << "      Tangent: " << tangent.x << ", " << tangent.y << ", " << tangent.z << endl;
				//cout << "      Bitangent: " << bitangent.x << ", " << bitangent.y << ", " << bitangent.z << endl;
			}



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

	// Load into Graphics Card

	// Load Data to Buffer
	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, size * 3 * sizeof(float), positions, GL_STATIC_DRAW); // Buffer detailed info: Buffer object 1 (bound to GL_ARRAY_BUFFER_ARB, usage hint is GL_STATIC_DRAW) will use VIDEO memory as the source for buffer object operations.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), indices, GL_STATIC_DRAW); // Buffer detailed info: Buffer object 1 (bound to GL_ARRAY_BUFFER_ARB, usage hint is GL_STATIC_DRAW) will use VIDEO memory as the source for buffer object operations.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// TUT 4 part 3 min 3
	glGenBuffers(1, &normalsBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normalsBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * 3 * sizeof(float), normals, GL_STATIC_DRAW); // Buffer detailed info: Buffer object 1 (bound to GL_ARRAY_BUFFER_ARB, usage hint is GL_STATIC_DRAW) will use VIDEO memory as the source for buffer object operations.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &tangentsBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tangentsBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * 3 * sizeof(float), tangents, GL_STATIC_DRAW); // Buffer detailed info: Buffer object 1 (bound to GL_ARRAY_BUFFER_ARB, usage hint is GL_STATIC_DRAW) will use VIDEO memory as the source for buffer object operations.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &bitangentsBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bitangentsBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * 3 * sizeof(float), bitangents, GL_STATIC_DRAW); // Buffer detailed info: Buffer object 1 (bound to GL_ARRAY_BUFFER_ARB, usage hint is GL_STATIC_DRAW) will use VIDEO memory as the source for buffer object operations.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// UV Buffer
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * 2 * sizeof(float), uvs, GL_STATIC_DRAW); // Buffer detailed info: Buffer object 1 (bound to GL_ARRAY_BUFFER_ARB, usage hint is GL_STATIC_DRAW) will use VIDEO memory as the source for buffer object operations.
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

	glBindBuffer(GL_ARRAY_BUFFER, tangentsBuffer);
	GLint tangentIndex = glGetAttribLocation(shader->programHandle, "vertexTangent_modelspace");
	if (tangentIndex != -1)
	{
		glEnableVertexAttribArray(tangentIndex);
		glVertexAttribPointer(tangentIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, bitangentsBuffer);
	GLint bitangentIndex = glGetAttribLocation(shader->programHandle, "vertexBitangent_modelspace");
	if (bitangentIndex != -1)
	{
		glEnableVertexAttribArray(bitangentIndex);
		glVertexAttribPointer(bitangentIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

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
	glDeleteBuffers(1, &tangentsBuffer);
	glDeleteBuffers(1, &bitangentsBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteVertexArrays(1, &vao);

	delete positions; positions = nullptr;
	delete normals; normals = nullptr;
	delete tangents; tangents = nullptr;
	delete bitangents; bitangents = nullptr;
	delete indices; indices = nullptr;
	delete uvs; uvs = nullptr;
	delete texture; texture = nullptr;
}

void Mesh::loadUniforms(Shader* shader, mat4x4 view, mat4x4 proj, mat4x4 globalPose)
{
	mat4x4 model = globalPose * updateTrans;
	mat4x4 mv = view * model;
	mat4x4 mvp = proj * mv;

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

	// Sun
	auto SunDirection_worldspace_location = glGetUniformLocation(shader->programHandle, "SunDirection_worldspace");
	glUniform3f(SunDirection_worldspace_location, SunDir.x, SunDir.y, SunDir.z);

	// Torch1
	auto Torch1Position_worldspace_location = glGetUniformLocation(shader->programHandle, "Torch1Position_worldspace");
	glUniform3f(Torch1Position_worldspace_location, torchPos[0].x, torchPos[0].y, torchPos[0].z);

	// Torch2
	auto Torch2Position_worldspace_location = glGetUniformLocation(shader->programHandle, "Torch2Position_worldspace");
	glUniform3f(Torch2Position_worldspace_location, torchPos[1].x, torchPos[1].y, torchPos[1].z);
	
	for (int i = 0; i < sizeof(torchPos) / sizeof(*torchPos); i++) {
		string nameString = "flameIntensity[" + std::to_string(i) + "]";
		auto flameIntensity_location = glGetUniformLocation(shader->programHandle, nameString.c_str());
		glUniform1f(flameIntensity_location, flameIntensity[i]);
	}

	// Diffuse
	auto diffuseColor = glGetUniformLocation(shader->programHandle, "diffuseColor");
	glUniform3f(diffuseColor, diffuse.r, diffuse.g, diffuse.b);

	// Specular
	auto specularColor = glGetUniformLocation(shader->programHandle, "specularColor");
	glUniform3f(specularColor, specular.r, specular.g, specular.b);

	// Normal
	auto normal_mapping_activated_location = glGetUniformLocation(shader->programHandle, "NORMAL_MAPPING");
	glUniform1i(normal_mapping_activated_location, NORMAL_MAPPING);

	// Fire and Shadows 1
	auto fire_and_shadows_1_activated_location = glGetUniformLocation(shader->programHandle, "FIRE_AND_SHADOWS_1");
	glUniform1i(fire_and_shadows_1_activated_location, FIRE_AND_SHADOWS_1);

	// Fire and Shadows 2
	auto fire_and_shadows_2_activated_location = glGetUniformLocation(shader->programHandle, "FIRE_AND_SHADOWS_2");
	glUniform1i(fire_and_shadows_2_activated_location, FIRE_AND_SHADOWS_2);

	// Texture
	auto tex_enabled = glGetUniformLocation(shader->programHandle, "hasTexture");
	if (hasTexture) {
		glUniform1i(tex_enabled, 1);

		texture->bind(TEXTURE_SLOT_MESH_DIFFUSE);

		auto modelTexture_location = glGetUniformLocation(shader->programHandle, "modelTexture");
		glUniform1i(modelTexture_location, TEXTURE_SLOT_MESH_DIFFUSE);
	}
	else {
		glUniform1i(tex_enabled, 0);
	}

	// Normal Texture
	auto normal_tex_enabled = glGetUniformLocation(shader->programHandle, "hasNormalTexture");
	if (hasNormalTexture && NORMAL_MAPPING) {
		glUniform1i(normal_tex_enabled, 1);

		normalTexture->bind(TEXTURE_SLOT_MESH_NORMAL);

		auto modelNormalTexture_location = glGetUniformLocation(shader->programHandle, "modelNormalTexture");
		glUniform1i(modelNormalTexture_location, TEXTURE_SLOT_MESH_NORMAL);
	}
	else {
		glUniform1i(normal_tex_enabled, 0);
	}
}

void Mesh::draw(Shader* shader, mat4x4 view, mat4x4 proj, mat4x4 globalPose, bool cull)
{ 
	if (!VIEWFRUSTUM_CULLING || !cull || isVisible(globalPose * updateTrans)){
		loadUniforms(shader, view, proj, globalPose);

		glBindVertexArray(vao);
		glDrawElements(CGUE_RENDER, size, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	else {
		NUMBER_OF_CULLED_MESHES++;
	}
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