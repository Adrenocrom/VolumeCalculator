#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

using namespace std;


struct img {
	int width;
	int height;

	float median;
	float mean;

	vector<float> _x;
	vector<float> _y;
	vector< vector<float> > data;
};

string makeStringClean(string str) {
	string res;

	for(auto it = str.begin(); it != str.end(); it++) {
		if(	*it == '0' ||
			*it == '1' ||
			*it == '2' ||
			*it == '3' ||
			*it == '4' ||
			*it == '5' ||
			*it == '6' ||
			*it == '7' ||
			*it == '8' ||
			*it == '9' ||
			*it == '.' ||
			*it == '-' ||
			*it == ' ') {
			res.push_back(*it);	
		}
		else {
			//printf("%d\n", *it);
		}
	}


	return res;
}

img readFromTxt(string filename) {
	img res;
	res.mean = 0.0f;
	vector<float>	_t;
	list<float>		_l;
	int cnt = 0;

	ifstream file(filename.c_str());
	if(!file.is_open()) {
		cerr<<"file not found!"<<endl;
		return res;	
	}

	string str_line;
	string tag;
	float temp;
	while(getline(file, str_line, '\n')) {

		std::replace( str_line.begin(), str_line.end(), '\t', ' ');
		std::replace( str_line.begin(), str_line.end(), '\r', ' ');
	
		stringstream line(makeStringClean(str_line));
	
		if(cnt == 0) {} 
		else if(cnt == 1) {
			while(getline(line, tag, ' ')) {
				if(tag.compare("") != 0)
					res._x.push_back(std::stof(tag));
			}

		}
		else {
			line >> tag;
			res._y.push_back(stof(tag));

			_t.clear();

			while(getline(line, tag, ' ')) {
				if(tag.compare("") != 0) {
					temp = stof(tag);
					_t.push_back(temp);
					_l.push_back(temp);
					res.mean += temp;
				}
			}

		
			res.data.push_back(_t);
		}
		
		cnt++;
	}

	_l.sort();
	cnt = 0;
	int size = (int)(((float) _l.size()) / 2.0f);
	for(auto it = _l.begin(); it != _l.end(); it++) {
		if(cnt == size) {
			res.median = *it;
			break;
		}

		cnt++;
	}

	res.mean /= (float)_l.size();

	res.width = res._x.size();
	res.height = res._y.size();

	cout<<"Resolution: "<<res.width<<" "<<res.height<<endl;

	file.close();
	return res;
}

int main(int argc, char* argv[]) {
	if(argc < 2 || argc > 4) {
		cout<<"wrong params"<<endl;
		cout<<"LINUX: ./volume <filename> [threshold] [dz]"<<endl;
		cout<<"WINDOOF: volume.exe <filename> [threshold} [dz]"<<endl;
		return -1;
	}
	string filename;
	filename = argv[1];
	img _img = readFromTxt(filename);

	float	T = 0.0f;

	if(argc == 3)
		T = stof(argv[2]);

	float	d = 0.0f;
	float	V = 0.0f;

	float	dx = _img._x[1] - _img._x[0];
	float	dy = _img._y[1] - _img._y[0];
	float	dz = 1.0f; //0.35f;

	if(argc == 4)
		dz = stof(argv[3]);

	cout<<"dx: "<<dx<<endl;
	cout<<"dy: "<<dy<<endl;
	cout<<"dz: "<<dz<<endl;

	cout<<"Median:            "<<_img.median<<endl;
	cout<<"Mean (Mittelwert): "<<_img.mean<<endl;

	for(int y = 0; y < _img.height; ++y) {
		for(int x = 0; x < _img.width; ++x) {
			d = T - _img.data[y][x];

			if(d > 0.0f)
				V += d * dz * dx * dy;
		}
	}
	
	cout<<"volume:            "<<V<<endl;

	return 0;
}
