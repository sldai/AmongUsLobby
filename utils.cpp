#include "utils.h"

using namespace std;

ostream& operator<< (ostream& out, const glm::vec3& vec) {
	out << "{"
		<< vec.x << " " << vec.y << " " << vec.z
		<< "}" << endl;

	return out;
}

ostream& operator<< (ostream& out, const glm::mat4& mat) {
	cout << "/" << "       " << "\\" << endl;
	cout << "|" << mat[0][0] << " " << mat[1][0] << " " << mat[2][0] << " " << mat[3][0] << "|" << endl;
	cout << "|" << mat[0][1] << " " << mat[1][1] << " " << mat[2][1] << " " << mat[3][1] << "|" << endl;
	cout << "|" << mat[0][2] << " " << mat[1][2] << " " << mat[2][2] << " " << mat[3][2] << "|" << endl;
	cout << "|" << mat[0][3] << " " << mat[1][3] << " " << mat[2][3] << " " << mat[3][3] << "|" << endl;
	cout << "\\" << "       " << "/" << endl;
	return out;
}

//ostream& operator<< (ostream& out, const glm::mat4& mat) {
//	cout << "/" << "       " << "\\" << endl;
//	cout << "|" << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << " " << mat[0][3] << "|" << endl;
//	cout << "|" << mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << " " << mat[1][3] << "|" << endl;
//	cout << "|" << mat[2][0] << " " << mat[2][1] << " " << mat[2][2] << " " << mat[2][3] << "|" << endl;
//	cout << "|" << mat[3][0] << " " << mat[3][1] << " " << mat[3][2] << " " << mat[3][3] << "|" << endl;
//	cout << "\\" << "       " << "/" << endl;
//	return out;
//}

stringstream split(string s, string delimiter) {

	size_t pos = 0;
	stringstream words;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		words << s.substr(0, pos) << " ";
		s.erase(0, pos + delimiter.length());
	}
	return words;
}


vector<glm::vec3> load_obj(string fname) {
	ifstream objFile(fname); // The obj file we are reading.
	vector<glm::vec3> points;

	// Check whether the file can be opened.
	if (objFile.is_open())
	{
		string line; // A line in the file.

		// Read lines from the file.
		while (getline(objFile, line))
		{
			// Turn the line into a string stream for processing.
			stringstream ss;
			ss << line;

			// Read the first word of the line.
			string label;
			ss >> label;

			// If the line is about vertex (starting with a "v").
			if (label == "v")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Process the point. For example, you can save it to a.
				points.push_back(point);
			}
		}
	}
	else
	{
		cerr << "Can't open the file " << fname << endl;
	}

	objFile.close();
	return points;
}

void load_obj(string fname, vector<glm::vec3> &vertices, // verted
	vector<glm::vec3> &colors,
	vector<glm::vec3> &vertices_normal, // vertex normal
	vector<glm::ivec3> &v_indices, vector<glm::ivec3> &vn_indices) {
	ifstream objFile(fname); // The obj file we are reading.
	vertices.clear();
	colors.clear();
	vertices_normal.clear();
	v_indices.clear();
	vn_indices.clear();

	// Check whether the file can be opened.
	if (objFile.is_open())
	{
		string line; // A line in the file.

		// Read lines from the file.
		while (getline(objFile, line))
		{
			// Turn the line into a string stream for processing.
			stringstream ss;
			ss << line;

			// Read the first word of the line.
			string label;
			ss >> label;

			// If the line is about vertex (starting with a "v").
			if (label == "v")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 v, c;
				ss >> v.x >> v.y >> v.z >> c.x >> c.y >> c.z;

				// Process the point. For example, you can save it to a.
				vertices.push_back(v);
				colors.push_back(c);
			}
			else if (label == "vn")
			{
				glm::vec3 vn;
				ss >> vn.x >> vn.y >> vn.z;
				vertices_normal.push_back(vn);
			}
			else if (label == "f")
			{
				glm::ivec3 vind, vnind;
				for (size_t i = 0; i < 3; i++)
				{
					string tmp;
					ss >> tmp;
					auto v_vn = split(tmp, "//");
					v_vn >> vind[i] >> vnind[i];
					vind[i] = vind[i] - 1;
					vnind[i] = vnind[i] - 1;
				}
				v_indices.push_back(vind);
				vn_indices.push_back(vnind);
			}
		}
	}
	else
	{
		cerr << "Can't open the file " << fname << endl;
	}

	objFile.close();
}


vector<glm::vec3> generate_point_clouds(int num)
{
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distrib(-1.0, 1.0);

	vector<glm::vec3> point_clouds(num);
	for (auto& point : point_clouds)
	{
		point = glm::vec3(distrib(generator), distrib(generator), distrib(generator));
	}
	return point_clouds;
}