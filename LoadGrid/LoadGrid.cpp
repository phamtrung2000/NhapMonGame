#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

vector<string> split(string line, string delimeter)
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}

int main()
{
	int left, top, right, bottom, x, y, type, cellwidth, cellheight, object_category;
	int bboxwidth; int bboxheight;

	ifstream ifs("InputGridScene1.1.txt", ios::in);
	ofstream ofs("OutputGridScene1.1.txt", ios::out);

	// current resource section flag
	int section = -1;

	char str[1024];
	while (ifs.getline(str, 1024))
	{
		string line(str);
		if (line[0] == '#') continue;	// skip comment lines
		if (line == "[CELLWIDTH,CELLHIEGHT]")
		{
			section = 0; continue;
		}
		if (line == "[OBJECTS]")
		{
			section = 1; continue;
		}
		if (line[0] == '[') { section = -1; continue; }

		switch (section)
		{
		case 0:
		{
			vector<string> tokens = split(line, "\t");
			cellwidth = atoi(tokens[0].c_str());
			cellheight = atoi(tokens[1].c_str());
		}
		break;

		case 1:
		{
			vector<string> tokens = split(line, "\t");
			object_category = atoi(tokens[0].c_str());
			type = atoi(tokens[1].c_str());
			x = atoi(tokens[2].c_str());
			y = atoi(tokens[3].c_str());
			bboxwidth = atoi(tokens[4].c_str());
			bboxheight = atoi(tokens[5].c_str());
			int a = int(min(20 - 1, int((x + bboxwidth) / cellheight)));
			ofs << " " << (int)(x * 1.0 / cellwidth) << " " << (int)(y / cellheight) << " " << a << " " << int(min(3 - 1, int((y + bboxheight) / cellheight))) << endl;

		}break;
		}
	}
	
	ifs.close();
	ofs.close();

	ifstream ifs1("InputGridScene1.3.txt", ios::in);
	ofstream ofs1("OutputGridScene1.3.txt", ios::out);

	// current resource section flag
	section = -1;

	char str1[1024];
	while (ifs1.getline(str1, 1024))
	{
		string line(str1);
		if (line[0] == '#') continue;	// skip comment lines
		if (line == "[CELLWIDTH,CELLHIEGHT]")
		{
			section = 0; continue;
		}
		if (line == "[OBJECTS]")
		{
			section = 1; continue;
		}
		if (line[0] == '[') { section = -1; continue; }

		switch (section)
		{
		case 0:
		{
			vector<string> tokens = split(line, "\t");
			cellwidth = atoi(tokens[0].c_str());
			cellheight = atoi(tokens[1].c_str());
		}
		break;

		case 1:
		{
			vector<string> tokens = split(line, "\t");
			object_category = atoi(tokens[0].c_str());
			type = atoi(tokens[1].c_str());
			x = atoi(tokens[2].c_str());
			y = atoi(tokens[3].c_str());
			bboxwidth = atoi(tokens[4].c_str());
			bboxheight = atoi(tokens[5].c_str());
			int a = int(min(20 - 1, int((x + bboxwidth) / cellheight)));
			ofs1 << " " << (int)(x * 1.0 / cellwidth) << " " << (int)(y / cellheight) << " " << a << " " << int(min(3 - 1, int((y + bboxheight) / cellheight))) << endl;

		}break;
		}
	}
	ifs1.close();
	ofs1.close();

	ifstream ifs2("InputGridSceneHidden1.1.txt", ios::in);
	ofstream ofs2("OutputGridSceneHidden1.1.txt", ios::out);

	// current resource section flag
	section = -1;

	char str2[1024];
	while (ifs2.getline(str2, 1024))
	{
		string line(str2);
		if (line[0] == '#') continue;	// skip comment lines
		if (line == "[CELLWIDTH,CELLHIEGHT]")
		{
			section = 0; continue;
		}
		if (line == "[OBJECTS]")
		{
			section = 1; continue;
		}
		if (line[0] == '[') { section = -1; continue; }

		switch (section)
		{
		case 0:
		{
			vector<string> tokens = split(line, "\t");
			cellwidth = atoi(tokens[0].c_str());
			cellheight = atoi(tokens[1].c_str());
		}
		break;

		case 1:
		{
			vector<string> tokens = split(line, "\t");
			object_category = atoi(tokens[0].c_str());
			type = atoi(tokens[1].c_str());
			x = atoi(tokens[2].c_str());
			y = atoi(tokens[3].c_str());
			bboxwidth = atoi(tokens[4].c_str());
			bboxheight = atoi(tokens[5].c_str());
			int a = int(min(6 - 1, int((x + bboxwidth) / cellheight)));
			ofs2 << " " << (int)(x * 1.0 / cellwidth) << " " << (int)(y / cellheight) << " " << a << " " << int(min(2 - 1, int((y + bboxheight) / cellheight))) << endl;

		}break;
		}
	}
	ifs2.close();
	ofs2.close();

	ifstream ifs3("InputGridSceneHidden1.3.txt", ios::in);
	ofstream ofs3("OutputGridSceneHidden1.3.txt", ios::out);

	// current resource section flag
	section = -1;

	char str3[1024];
	while (ifs3.getline(str3, 1024))
	{
		string line(str3);
		if (line[0] == '#') continue;	// skip comment lines
		if (line == "[CELLWIDTH,CELLHIEGHT]")
		{
			section = 0; continue;
		}
		if (line == "[OBJECTS]")
		{
			section = 1; continue;
		}
		if (line[0] == '[') { section = -1; continue; }

		switch (section)
		{
		case 0:
		{
			vector<string> tokens = split(line, "\t");
			cellwidth = atoi(tokens[0].c_str());
			cellheight = atoi(tokens[1].c_str());
		}
		break;

		case 1:
		{
			vector<string> tokens = split(line, "\t");
			object_category = atoi(tokens[0].c_str());
			type = atoi(tokens[1].c_str());
			x = atoi(tokens[2].c_str());
			y = atoi(tokens[3].c_str());
			bboxwidth = atoi(tokens[4].c_str());
			bboxheight = atoi(tokens[5].c_str());
			int a = int(min(7 - 1, int((x + bboxwidth) / cellheight)));
			ofs3 << " " << (int)(x * 1.0 / cellwidth) << " " << (int)(y / cellheight) << " " << a << " " << int(min(3 - 1, int((y + bboxheight) / cellheight))) << endl;

		}break;
		}
	}
	ifs3.close();
	ofs3.close();

	system("pause");
}