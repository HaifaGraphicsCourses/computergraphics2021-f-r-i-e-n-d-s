#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Utils.h"
#include "../../build/Transformations.h"

glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

std::shared_ptr<MeshModel> Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	glm::vec3 Vertex;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::mat4x4 Transformation;
	std::ifstream ifile(filePath.c_str());
	float Min_X = FLT_MAX, Max_X = FLT_MIN, Max_Y = FLT_MIN, Min_Y = FLT_MAX, Max_Z = FLT_MIN, Min_Z = FLT_MAX,MaxIndex;
	//parameters to assign a scaling values
	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			Vertex = Utils::Vec3fFromStream(issLine);
			if (Vertex.x > Max_X)
				Max_X = Vertex.x;
			if (Vertex.x < Min_X)
				Min_X = Vertex.x;

			if (Vertex.y > Max_Y)
				Max_Y = Vertex.y;
			if (Vertex.y < Min_Y)
				Min_Y = Vertex.y;

			if (Vertex.z > Max_Z)
				Max_Z = Vertex.z;
			if (Vertex.z < Min_Z)
				Min_Z = Vertex.z;

			vertices.push_back(Vertex);
		}
		else if (lineType == "vn")
		{
			normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			// TODO: Handle texture coordinates
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}
		MaxIndex = Max_X > Max_Y ? Max_X : Max_Y;
		MaxIndex = MaxIndex > Max_Z ? MaxIndex : Max_Z;
		glm::vec4 ModelCenter((Min_X + Max_X) / 2, (Min_Y + Max_Y) / 2, (Min_Z + Max_Z) / 2, 1);
		float ScalingParameter = 300 / MaxIndex;
		glm::mat4x4 scaling = Transformations::ScalingTransformation(ScalingParameter, ScalingParameter, ScalingParameter);
		glm::mat4x4 translateObjectToCenter = Transformations::TranslationTransformation(-Min_X, -Min_Y, -Min_Z);
		Transformation = scaling * translateObjectToCenter;
	return std::make_shared<MeshModel>(faces, vertices, normals, Utils::GetFileName(filePath),Transformation);
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}