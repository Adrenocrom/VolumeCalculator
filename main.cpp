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

	double median;
	double mean;

	vector<double> _x;
	vector<double> _y;
	vector<double> data;
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
	res.mean = 0.0;
	list<double>	_l;

	ifstream file(filename.c_str());
	if(!file.is_open()) {
		cerr<<"file not found!"<<endl;
		return res;	
	}

	string str_line;
	string tag;
	double temp;

	// push data back
	getline(file, str_line, '\n');
	getline(file, str_line, '\n');
	std::replace( str_line.begin(), str_line.end(), '\t', ' ');
	stringstream line(makeStringClean(str_line));
	while(getline(line, tag, ' ')) {
		if(tag.compare("") != 0) {
			res._x.push_back(stod(tag));
		}
	}

	
	while(getline(file, str_line, '\n')) {
		std::replace( str_line.begin(), str_line.end(), '\t', ' ');
		stringstream line(makeStringClean(str_line));
		line >> tag;
			
		res._y.push_back(stod(tag));

		while(getline(line, tag, ' ')) {
			if(tag.compare("") != 0) {
				temp = stod(tag);
				res.data.push_back(temp);
				res.mean += temp;

				_l.push_back(temp);
			}
		}
	}


	_l.sort();
	int cnt = 0;
	int size = (int)(((double) _l.size()) / 2.0);
	for(auto it = _l.begin(); it != _l.end(); it++) {
		if(cnt == size) {
			res.median = *it;
			break;
		}

		cnt++;
	}

	res.mean /= (double)_l.size();

	res.width = res._x.size();
	res.height = res._y.size();

	cout<<"Resolution: "<<res.width<<" "<<res.height<<endl;

	file.close();
	return res;
}

int main(int argc, char* argv[]) {
	if(argc < 2 || argc > 4) {
		cout<<"wrong params"<<endl;
		cout<<"LINUX: ./volume <filename> [border = 50] [min = 100]"<<endl;
		cout<<"WINDOOF: volume.exe <filename> [border = 50] [min = 100]"<<endl;
		return -1;
	}
	string filename;
	filename = argv[1];
	img _img = readFromTxt(filename);

	int o = 50; // offset
	if(argc == 3)
		o = stoi(argv[2]);
	int ox = _img.width - o -1;
	int oy = _img.height - o -1;

	int	min = 99;
	if(argc == 4)
		min = stoi(argv[3])-1;

	double	d = 0.0;
	double	V = 0.0;

	double	dx = _img._x[1] - _img._x[0];
	double	dy = _img._y[1] - _img._y[0];

	cout<<"dx: "<<dx<<endl;
	cout<<"dy: "<<dy<<endl;

	cout<<"Median:            "<<_img.median<<endl;
	cout<<"Mean (Mittelwert): "<<_img.mean<<endl;

	list<double> _l;
	double mean = 0;
	double median = 0;

	for(int y = 0; y < _img.height; ++y) {
		for(int x = 0; x < _img.width; ++x) {
			if(x < o || x > ox && y < o || y > oy) {
			   mean += _img.data[y * _img.height + x];
				_l.push_back(_img.data[y * _img.width + x]);
			}
		}
	}
	
	int N = _l.size();
	mean /= (double)N;
	
	_l.sort();
	
	int cnt = 0;
	for(auto it = _l.begin(); it != _l.end(); it++) {
		if(cnt == min) {
			median = *it;
			break;
		}
		cnt++;
	}


	cout<<"outer mean: "<<mean<<endl;
	cout<<"outer median: "<<median<<endl;

	for(int y = 0; y < _img.height; ++y) {
		for(int x = 0; x < _img.width; ++x) {
			double& p = _img.data[y * _img.width + x];

			if(p < median) {
				d = mean - p;

				if(d > 0.0) {
					V += d;
				}
			}
		}
	}


	V *= dx * dy;
	cout<<"volume:            "<<V<<endl;

	return 0;
}
