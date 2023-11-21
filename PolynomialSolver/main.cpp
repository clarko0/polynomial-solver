#include <iostream>
#include <vector>
#include <tuple>
#include <regex>
#include <string>
#include <algorithm>

using namespace std;

vector<string> equations = {
        "x^2-5x+2=5x^10-4",
        "3x^4+2x^3-x^2+5=0",
        "4x^3-x^2+7x-1=2x^5+3",
        "x^3+6x^2-3x+2=4x^4-5x^2+1",
        "2x^4+4x^3-6x^2+x-3=0",
        "5x^3+2x^2+8=3x^4+x^2-7",
        "3x^2-x+5=2x^6+x^3-4x^2",
        "2x^3-4x+1=x^5+3x^2-6",
        "x^3+x^2+3x-2=0",
        "4x^2-2x+7=3x^4-x^2+5",
        "2x^3+5x^2-x=3x^6+2x^3-x^2",
        "3x^2-x+6=x^4+2x^2-3",
        "x^3-2x+4=0",
        "5x^3+x-3=2x^5-x^3+4",
        "4x^2-x^2+2x-5=0",
        "2x^3+3x-1=x^4+5x^2-2",
        "x^3+2x+5=0",
        "3x^2-x^2+4x-2=x^4+3x^2-1",
        "4x^3+2x-1=0",
        "x^3-x^2+3x+7=2x^4-x^2+5"
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

vector<int> differentiate_equation(vector<int> equation) {
    vector<int> res;

    for (int i = 0; i < equation.size(); i++) {
        res.push_back(0);

        if (i == 0) continue;
        else res[i - 1] = equation[i] * i;
    }

    return res;
}

int main() {

    for (string i : equations) {
        vector<int> equation = parse_equation(i);
        vector<int> dydx_equation = differentiate_equation(equation);

    }

    return 0;
}
