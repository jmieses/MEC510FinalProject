#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <fstream>

using namespace std;
string files[] = { "./test/a170503.csv" , "./test/a170504.csv" , "./test/a170505.csv" ,"./test/a170506.csv" }; // files in the project. Relative path.
const double pi = 3.14159265; // pi is declared as a global constant value.
int number_of_files = 4;

class Windspeed { // class to store the wind speed data from files, and wind properties.
private:
	vector <double> ws80, wd80; // vector container for the wind speed data.
	vector <string> saved_line;
	ifstream file1; // ifstream file object to read the files.
	string line;
	double density, Cp;
public:
	Windspeed(){}
	void wind_properties(double ndensity, double nCp) // initialize the wind properties with data entered by the user. 
	{
		density = ndensity;
		Cp = nCp;
	} 
	void store_data(); // functions of the Windspeed class to manipulate the data. 
	void store_lines();
	void display_data();
	void display_lines();
	int find_data(string date, string time);
	double get_ws80(int i);
	double get_wd80(int i);
	double get_density() { return density; }
	double get_Cp() { return Cp; }
};
////////////////////////////////////////////////////////////////////////////
class Turbine:public Windspeed {
private:
	double radius;
public:
	void turbine_radius(double nradius) { radius = nradius; }

	double mean_ws80(string date1, string time1, string date2, string time2);
	double rms_ws80(string date1, string time1, string date2, string time2);
	double power_ws80(string date1, string time1, string date2, string time2);

	double mean_wd80(string date1, string time1, string date2, string time2);
	double rms_wd80(string date1, string time1, string date2, string time2);
};
//////////////////////////////////////////////////////////////////////////////
int main()
{
	double density, Cp, radius; // variables for  the user to input turbine wind properties. 
	Turbine tbs; // create turbine object. 
	string choice = "y";
	cout << "This program calculates the average and rms of the speed and direction of a wind turbine, ";
	cout << "and the power produced by the wind turbine" << endl;
	do {
		std::cout << "Enter wind density: "; std::cin >> density; std::cout << std::endl;
		std::cout << "Enter Cp: "; std::cin >> Cp; std::cout << std::endl;
		std::cout << "Enter radius: "; std::cin >> radius; std::cout << std::endl;
		// create a turbine object and access its data members.
		tbs.store_data();
		tbs.store_lines();
		tbs.wind_properties(density, Cp); // pass the wind properties to the object private members. 
		tbs.turbine_radius(radius);
		string date1, time1, date2, time2; // declare string variables to store the date and time. 
		std::cout << "Choose a starting date from the following options: 5/3/17, 5/4/17, 5/5/17, 5/6/17" << std::endl;
		std::cin >> date1;
		std::cout << "Enter a starting time of the form: 0:00:00" << std::endl;
		std::cin >> time1;
		std::cout << "Choose an ending date from the following options: 5/3/17, 5/4/17, 5/5/17, 5/6/17" << std::endl;
		std::cin >> date2;
		std::cout << "Enter an ending time of the form: 0:00:00" << std::endl;
		std::cin >> time2;
		if (tbs.find_data(date1, time1) == 0.0 || tbs.find_data(date2, time2) == 0.0) {
			std::cout << "Ups! One of the dates or the times that you entered are not in the correct form" << std::endl;
			std::cout << "Make sure the date is of the form m/d/yy, and time of the form 0:00:00" << std::endl;
			continue;
		}

		std::cout << "mean ws80: " << tbs.mean_ws80(date1, time1, date2, time2) << endl; // display the quantities to the user. 
		std::cout << "rms ws80: " << tbs.rms_ws80(date1, time1, date2, time2) << endl;
		

		std::cout << "mean wd80: " << tbs.mean_wd80(date1, time1, date2, time2) << std::endl; // display the quantities to the user. 
		std::cout << "rms wd80: " << tbs.rms_wd80(date1, time1, date2, time2) << std::endl;
		
		std::cout << "Power produced: " << tbs.power_ws80(date1, time1, date2, time2) << std::endl;

		std::cout << "Do you want to calculate another set of dates (y/n): "; std::cin >> choice;
	} while (choice == "y" || choice == "yes" );

	std::system("pause");
	return 0;
}

// function definitions of Windspeed class and Turbine class. 
void Windspeed::store_data() { // store the data from the files. 
	vector <double>* ws8 = &ws80;
	vector <double>* wd8 = &wd80;

	for (int i = 0; i < number_of_files;i++)
	{
		//cout << i << endl;
		file1.open(files[i]);
		getline(file1, line);
		while (!file1.eof())
		{
			getline(file1, line);
			
			int count = 0;
			string store = line;
			for (int j = 0, ilen = store.length();j < ilen;j++) {
				int index = j;
				if (store[j] == ',' && store[j + 1] == ',')
				{
					getline(file1, line);
					store = line;
				}
				if (store[index] == ',')
				{
					count++;
					if (count == 2)
					{
						string  stringsum = "\0";
						do {
							stringsum += store[index + 1];
							index++;
							if (index + 1 > store.length()) break;
						} while (store[index + 1] != ',');
						if (store[index + 2] == ',') // check for empty cell, and put a zero there.
							ws80.push_back(0.0);
						else
						ws80.push_back(stod(stringsum));
					
					}
					else if (count == 3)
					{
						if (store[index + 1] == ' ') { // check for empty cell, and put a zero there.
							wd80.push_back(0.0);
						}
						else {
							string stringsum1 = "\0";
							do {
								stringsum1 += store[index + 1];
								index++;
								if (index + 1 > store.length()) break;
							} while (store[index + 1] != '\0');
							wd80.push_back(stod(stringsum1));
						}
						
					}
				}
			}
		}
		file1.close();
	}

}

void Windspeed::display_data() {// function to display the data obtained from the files. 

	for (int i = 0, ilen = ws80.size(); i < ilen; i++)
	{
		cout << ws80[i] << endl;
	}
	for (int j = 0, ilen = wd80.size();j < ilen; j++)
	{
		cout << wd80[j] << endl;
	}
}

void Windspeed::display_lines() { // function to display the lines obtained from the files. 
	for (int i = 0; i < saved_line.size();i++)
	{
		cout << saved_line[i] << endl;
		
	}
	cout << saved_line.size() << endl;
}

int Windspeed::find_data(string date, string time) { // detect the data enter by the user. 
	string temp, dum = "\0", tm = "\0";
	for (int i = 1, ilen = saved_line.size()+1;i < ilen; i++)
	{
		temp = saved_line[i-1];
		int j = 0;
		while (temp[j] != ',')
		{
			dum += temp[j];
			j++;
		}
		while (temp[j+1] != ',')
		{
			tm += temp[j+1];	
			j++;
		}
		if (dum == date && tm == time)
		{
			return i;
		}

		dum = "\0";
		tm = "\0";
	}
	
	return 0;
}

void Windspeed::store_lines() { // store the lines of the files for further use. 
	vector <string>* temp = &saved_line;
	for (int i = 0; i < 4; i++) {
		file1.open(files[i]);
		getline(file1, line);
		while (!file1.eof()) {
			getline(file1, line);
			saved_line.push_back(line);
		}
		file1.close();
	}
}

double Windspeed::get_ws80(int i) {
	return ws80[i];
} // get the wind speed data.

double Windspeed::get_wd80(int i) {
	return wd80[i];
}// get the wind direction data. 

// calculation of mean, rms and power functions. 

double Turbine::mean_ws80(string date1, string time1, string date2, string time2) // function to calculate the average value of the wind speed.
{
	int first_line = find_data(date1,time1)-1;
	int last_line = find_data(date2, time2)-1;
	
	double sum = 0;
	for (int i = first_line; i < last_line+1; i++)
	{
		sum += get_ws80(i);
	}
	return sum/(last_line+1-first_line);
}

double Turbine::rms_ws80(string date1, string time1, string date2, string time2) { // function to calculate the rms of the wind speed. 
	double difference = 0, mean = mean_ws80(date1,time1,date2,time2);
	int first_line = find_data(date1, time1)-1;
	int last_line = find_data(date2, time2)-1;
	
	for (int i = first_line; i < last_line + 1; i++)
	{
		difference += pow(get_ws80(i) - mean,2);
	}
	return pow(difference / (last_line + 1 - first_line), 0.5);
}

double Turbine::power_ws80(string date1, string time1, string date2, string time2) {// function to calculate the power produced by the turbine. 
	int first_line = find_data(date1, time1)-1;
	int last_line = find_data(date2, time2)-1;
	
	double U = mean_ws80(date1, time1, date2, time2);
	return 0.5*get_density()*get_Cp()*pi*pow(radius, 2)*pow(U, 3);
}

double Turbine::mean_wd80(string date1, string time1, string date2, string time2) // function to calculate wind direction average value.
{
	int first_line = find_data(date1, time1)-1;
	int last_line = find_data(date2, time2)-1;
	
	double sum = 0;
	for (int i = first_line; i < last_line + 1; i++)
	{
		sum += get_wd80(i);
	}
	return sum / (last_line + 1 - first_line);
}

double Turbine::rms_wd80(string date1, string time1, string date2, string time2) { // function to calculate rms of wind direction. 
	double difference = 0, mean = mean_wd80(date1, time1, date2, time2);
	int first_line = find_data(date1, time1)-1;
	int last_line = find_data(date2, time2)-1;
	
	for (int i = first_line; i < last_line + 1; i++)
	{
		difference += pow(get_wd80(i) - mean, 2);
	}
	return pow(difference / (last_line + 1 - first_line), 0.5);
}

