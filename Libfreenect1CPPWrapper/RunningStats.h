/* 
 *    Copyright [2015] Olaf - blinky0815 - christ ]
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 * 
 * File:   RunningStats.h
 * Author: Olaf christ (Blinky0815)
 *
 * Created on December 7, 2015, 2:58 PM
 * 
 * See B. P. Welford, 1962 and Donald Knuth’s Art of Computer Programming, Vol 2, page 232, 3rd edition.
 * 
 * Note on a Method for Calculating Corrected Sums of Squares and Products
 * DOI:10.1080/00401706.1962.10490022
 * B. P. Welforda pages 419-420
 * 
 * Furthermore, see: http://www.johndcook.com/blog/skewness_kurtosis/ 
 */

#ifndef RUNNINGSTATS_H
#define RUNNINGSTATS_H

#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

class RunningStats {
public:
    long long n;
    long long nv;
    double m1, m2, m3, m4;
    vector<double> mv1, mv2, mv3, mv4;

    RunningStats() {
        clear();
    }

    RunningStats(int size) {
        mv1.resize(size, 0.0);
        mv2.resize(size, 0.0);
        mv3.resize(size, 0.0);
        mv4.resize(size, 0.0);
        clear();
    }

    void clear() {
        n = 0;
        nv = 0;
        m1 = m2 = m3 = m4 = 0.0;
        mv1.clear();
        mv2.clear();
        mv3.clear();
        mv4.clear();
    }

    void push(vector<double> values) {
        if (values.size() != mv1.size()) {
            cout << "vectors must have same the size" << endl;
        } else {
            long long nv1 = nv;
            nv++;
            for (int i = 0; i < values.size(); i++) {
                double delta;
                double delta_n;
                double delta_n2;
                double term;
                delta = values.at(i) - mv1.at(i);
                delta_n = delta / nv;
                delta_n2 = delta_n * delta_n;
                term = delta * delta_n * nv1;
                mv1.at(i) += delta_n;
                mv4.at(i) += term * delta_n2 * (nv * nv - 3 * nv + 3) + 6 * delta_n2 * mv2.at(i) - 4 * delta_n * mv3.at(i);
                mv3.at(i) += term * delta_n * (nv - 2) - 3 * delta_n * mv2.at(i);
                mv2.at(i) += term;
            }
        }
    }

    void push(double x) {
        double delta;
        double delta_n;
        double delta_n2;
        double term;
        long long n1 = n;

        n++;
        delta = x - m1;
        delta_n = delta / n;
        delta_n2 = delta_n * delta_n;
        term = delta * delta_n * n1;
        m1 += delta_n;
        m4 += term * delta_n2 * (n * n - 3 * n + 3) + 6 * delta_n2 * m2 - 4 * delta_n * m3;
        m3 += term * delta_n * (n - 2) - 3 * delta_n * m2;
        m2 += term;
    }

    long long numDataValues() const {
        return n;
    }

    double mean() const {
        return m1;
    }

    double variance() const {
        return m2 / (n - 1.0);
    }

    double standardDeviation() const {
        return std::sqrt(variance());
    }

    double skewness() const {
        return std::sqrt(double(n)) * m3 / pow(m2, 1.5);
    }

    double kurtosis() const {
        return double(n) * m4 / (m2 * m2) - 3.0;
    }

    RunningStats& operator+(const RunningStats& b) {

        long long combined_n = n + b.n;
        double combined_m1, combined_m2, combined_m3, combined_m4;

        double delta = b.m1 - m1;
        double delta2 = delta*delta;
        double delta3 = delta*delta2;
        double delta4 = delta2*delta2;

        combined_m1 = (n * m1 + b.n * b.m1) / combined_n;
        combined_m2 = m2 + b.m2 + delta2 * n * b.n / combined_n;
        combined_m3 = m3 + b.m3 + delta3 * n * b.n * (n - b.n) / (combined_n * combined_n);
        combined_m3 += 3.0 * delta * (n * b.m2 - b.n * m2) / combined_n;
        combined_m4 = m4 + b.m4 + delta4 * n * b.n * (n * n - n * b.n + b.n * b.n) / (combined_n * combined_n * combined_n);
        combined_m4 += 6.0 * delta2 * (n * n * b.m2 + b.n * b.n * m2) / (combined_n * combined_n) + 4.0 * delta * (n * b.m3 - b.n * m3) / combined_n;

        this->m1 = combined_m1;
        this->m2 = combined_m2;
        this->m3 = combined_m3;
        this->m4 = combined_m4;
    }

    virtual ~RunningStats() {
    }
private:

};

#endif /* RUNNINGSTATS_H */

