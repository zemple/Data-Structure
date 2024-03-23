/**
 * @file schedule.cpp
 * Exam scheduling using graph coloring
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <unordered_map>

#include "schedule.h"
#include "utils.h"
#include <algorithm>

using namespace std;
Graph graph;
/**
 * Given a filename to a CSV-formatted text file, create a 2D vector of strings where each row
 * in the text file is a row in the V2D and each comma-separated value is stripped of whitespace
 * and stored as its own string. 
 * 
 * Your V2D should match the exact structure of the input file -- so the first row, first column
 * in the original file should be the first row, first column of the V2D.
 *  
 * @param filename The filename of a CSV-formatted text file. 
 */
V2D file_to_V2D(const string & filename){
    string file = file_to_string(filename);
    vector<string> lines;
    int n = split_string(file, '\n', lines);
    V2D v = vector<vector<string>>(n, vector<string>(0));
    for (int i = 0; i < n; i++) {
        std::vector<string> line;
        int m = split_string(lines[i], ',', line);
        for (int j = 0; j < m; j++) {
            v[i].push_back(trim(line[j]));
        }
    }
    return v;
}

/**
 * Given a course roster and a list of students and their courses, 
 * perform data correction and return a course roster of valid students (and only non-empty courses).
 * 
 * A 'valid student' is a student who is both in the course roster and the student's own listing contains the course
 * A course which has no students (or all students have been removed for not being valid) should be removed
 * 
 * @param cv A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param student A 2D vector of strings where each row is a student ID followed by the courses they are taking
 */
V2D clean(const V2D & cv, const V2D & student){
    // YOUR CODE HERE
    V2D toReturn(cv.size());
    int row = 0;
    for (auto i : cv) {
        string name = i[0];
        if (!toReturn[row].empty()) {
            toReturn[row][0] = name;
        } else {
            toReturn[row].push_back(name);
        }
        for (auto j : i) {
            for (auto s : student) {
                if (j == s[0]) {
                    for (auto course : s) {
                        if (course == name) {
                            toReturn[row].push_back(j);
                        } else {
                            continue;
                        }
                    }
                } else {
                    continue;
                }
            }
        }
        if (toReturn[row].size() != 1) {
            row++;
        } else {
            toReturn.pop_back();
        }
    }
    return toReturn;
}

/**
 * Given a collection of courses and a list of available times, create a valid scheduling (if possible).
 * 
 * A 'valid schedule' should assign each course to a timeslot in such a way that there are no conflicts for exams
 * In other words, two courses who share a student should not share an exam time.
 * Your solution should try to minimize the total number of timeslots but should not exceed the timeslots given.
 * 
 * The output V2D should have one row for each timeslot, even if that timeslot is not used.
 * 
 * As the problem is NP-complete, your first scheduling might not result in a valid match. Your solution should 
 * continue to attempt different schedulings until 1) a valid scheduling is found or 2) you have exhausted all possible
 * starting positions. If no match is possible, return a V2D with one row with the string '-1' as the only value. 
 * 
 * @param courses A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param timeslots A vector of strings giving the total number of unique timeslots
 */
V2D schedule(const V2D &courses, const std::vector<std::string> &timeslots) {
    V2D result;
    map<string, int> cc;
    graph = Create(courses);
    vector<string> q;
    for (unsigned i = 0; i < courses.size(); i++) {
        q.push_back(courses[i][0]);
    }
    sort(q.begin(), q.end(), [](string a, string b) { 
		return graph[a].size() > graph[b].size();
    });
    for (unsigned i = 0; i < q.size();) {
        vector<string> vec = graph[q[i]];
        vector<bool> visited(timeslots.size(), false);
        for (unsigned j = 0; j < vec.size(); j++) {
            if (cc.count(vec[j]) > 0) {
                visited[cc[vec[j]]] = true; 
            }
        }
        bool success = false;
        for (unsigned j = 0; j < visited.size(); j++) {
            if (!visited[j]) {
				cc[q[i]] = j;
				success = true;
				break;
            }
        }
        if (success) {
		    i++;
        } else {
		    result.push_back(vector<string>(1, "-1"));
		    return result;
        }
    }
    for (unsigned i = 0; i < timeslots.size(); i++) {
		vector<string> vec;
		vec.push_back(timeslots[i]);
		result.push_back(vec);
    }
    for (auto edge : cc) {
		int color = edge.second;
		result[color].push_back(edge.first);
    }
    return result;
}

Graph Create(const V2D &courses) {
	Graph result;
	Graph sc;
	for (unsigned i = 0; i < courses.size(); i++) {
		string course = courses[i][0];
		for (unsigned j = 1; j < courses[i].size(); j++) {
			string student = courses[i][j];
			if (sc.count(student) == 0) {
				sc[student] = vector<string>();
			}
			sc[student].push_back(course);
		}
	}
	for (auto edge : sc) {
		vector<string> vec = edge.second;
		for (unsigned i = 0; i < vec.size(); i++) {
			if (result.count(vec[i]) == 0) {
				result[vec[i]] = vector<string>();
			}
			for (unsigned j = 0; j < vec.size(); j++) {
				if (i != j) {
					result[vec[i]].push_back(vec[j]);
				}
			}
		}
	}
	return result;
}


