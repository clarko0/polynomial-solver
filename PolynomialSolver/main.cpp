#include <iostream>
#include <vector>
#include <tuple>
#include <regex>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;

vector<string> equations = {
    "x^2-5x-90=0",
    "x^2-5x+2=5x^10-4"
};

vector<string> split(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

bool isInString(string main, string substr) {
    return main.find(substr) != string::npos;
}

regex polynomial_part_pattern("[+-]?[^-+]+");

vector<string> parse_section(const string& input) {
    sregex_iterator iter(input.begin(), input.end(), polynomial_part_pattern);
    sregex_iterator end;

    vector<string> result;
    for (; iter != end; ++iter) {
        result.push_back(iter->str());
    }

    return result;
}

int get_largest_power(vector <vector <tuple<int, int>>> data) {

    int largest = 0;

    for (vector <tuple<int, int>> i : data) {
        for (tuple<int, int> j : i) {

            int power = get<1>(j);

            if (power > largest) largest = power;
        }
    }

    return largest;
}

vector<int> parse_equation(string equation)
{
    vector<int> res;

    size_t pos = 0;
    string token;

    vector <string> equation_parts = split(equation, "=");

    vector <vector <tuple<int, int>>> abs_polynomial_data;

    for (string i : equation_parts) {

        vector <tuple<int, int>> section;

        vector<string> parts = parse_section(i);

        for (string part : parts) {


            int coefficient;
            int power;

            bool isConstant = !isInString(part, "x");

            if (isConstant) {
                power = 0;
                coefficient = stoi(part);
            }
            else {
                vector<string> split_1 = split(part, "x^");

                if (split_1.size() == 1) {
                    power = 1;
                    split_1[0].erase(remove(split_1[0].begin(), split_1[0].end(), 'x'), split_1[0].end());
                }
                else {
                    power = stoi(split_1.at(split_1.size() - 1));
                }

                string coefficient_val = split_1.at(0);

                if (coefficient_val == "+" || coefficient_val == "") {
                    coefficient = 1;
                }
                else if (coefficient_val == "-") {
                    coefficient = -1;
                }
                else {
                    coefficient = stoi(coefficient_val);
                }
            }

            section.push_back(make_tuple(coefficient, power));
        }

        abs_polynomial_data.push_back(section);
    }

    int largest_power = get_largest_power(abs_polynomial_data);

    for (int index = 0; index < largest_power + 1; index++) {
        res.push_back(0);
    }

    for (int index = 0; index < abs_polynomial_data.size(); index++) {

        bool is_first_index = index == 0;

        for (tuple<int, int> j : abs_polynomial_data[index]) {

            int power = get<1>(j);
            int coefficient = get<0>(j);

            if (is_first_index) res[power] += coefficient;
            else res[power] -= coefficient;
        }
    }

    return res;
}

double calculate_equation(vector<int> equation, double x) {
    double total = 0;
    for (int i = 0; i < equation.size(); i++) {
        total += equation[i] * pow(x, i);
    }

    return total;
}

vector<int> differentiate_equation(vector<int> equation) {
    vector<int> res;

    for (int i = 0; i < equation.size(); i++) {
        res.push_back(0);

        if (i == 0) continue;
        else res[i - 1] = equation[i] * i;
    }

    return res;
}

double newtons_method(vector<int> y, vector<int> dydx, double x)
{
    double rightPart = calculate_equation(y, x) / calculate_equation(dydx, x);

    return x - rightPart;
}

double random_int(int lower, int upper) {
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> distribution(lower, upper - 1);

    int random_int = distribution(gen);

    return random_int;
}

void solve_equation(string equation, double precision) {
    auto start_time = chrono::high_resolution_clock::now();

    string raw_equation = equation;

    vector<int> y = parse_equation(raw_equation);
    vector<int> dydx = differentiate_equation(y);

    vector<double> x_values;

    int max_iterations = 500;
    int iteration = 0;

    while (iteration <= max_iterations) {
        if (x_values.size() == y.size() - 1) break;

        double x = random_int(-20000000, 2000000);
        double prevX = 0;

        while (true) {
            x = newtons_method(y, dydx, x);

            double diff = x - prevX;
            prevX = x;
            if (diff >= -precision && diff <= precision) {
                bool inSet = false;
                for (int i = 0; i < x_values.size(); i++) {
                    if (x_values[i] >= x - precision && x_values[i] <= x + precision) {
                        inSet = true;
                        break;
                    }
                }

                if (!inSet) x_values.push_back(x);

                break;
            }
        }

        iteration++;
    }

    auto end_time = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

    for (double i : x_values) {
        cout << "x = " << i << endl;
    }

    cout << "time taken: " << duration.count() << "ms" << endl;
}

int main() {

    for (string equation : equations) {
        solve_equation(equation, 0.000001);
    }


    return 0;
}
