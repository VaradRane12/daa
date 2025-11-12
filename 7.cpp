#include <bits/stdc++.h> 
using namespace std; 
 
// Structure for room details 
struct Room { 
    string name; 
    int capacity; 
    bool available; 
}; 
 
// Structure for course details 
struct Course { 
    string name; 
    int numStudents; 
    int color; // time slot 
    string assignedRoom; 
}; 
 
// Function to build the conflict graph 
void buildConflictGraph(map<string, vector<string>>& studentCourses, 
                        map<string, vector<string>>& adjList) { 
    for (auto& entry : studentCourses) { 
        const vector<string>& courses = entry.second; 
        for (int i = 0; i < courses.size(); i++) { 
            for (int j = i + 1; j < courses.size(); j++) { 
                string c1 = courses[i]; 
                string c2 = courses[j]; 
                adjList[c1].push_back(c2); 
                adjList[c2].push_back(c1); 
            } 
        } 
    } 
} 
 
// Greedy Graph Colouring Algorithm 
void greedyColoring(map<string, vector<string>>& adjList, 
                    map<string, Course>& courses) { 
    // Sort vertices by degree (Welsh-Powell optimization) 
    vector<pair<string, int>> degreeList; 
    for (auto& entry : adjList) { 
        degreeList.push_back({entry.first, (int)entry.second.size()}); 
    } 
    sort(degreeList.begin(), degreeList.end(), 
         [](auto& a, auto& b) { return a.second > b.second; }); 
 
    int colorCount = 0; 
    for (auto& item : degreeList) { 
        string course = item.first; 
        set<int> usedColors; 
        for (auto& neighbor : adjList[course]) { 
            if (courses[neighbor].color != -1) 
                usedColors.insert(courses[neighbor].color); 
        } 
 
        int color = 0; 
        while (usedColors.find(color) != usedColors.end()) color++; 
        courses[course].color = color; 
        colorCount = max(colorCount, color + 1); 
    } 
 
    cout << "\nTotal Exam Slots (Chromatic Number): " << colorCount << "\n\n"; 
} 
 
// Room Allocation per time slot 
void allocateRooms(map<string, Course>& courses, 
                   vector<Room>& rooms) { 
    // Group courses by time slot 
    map<int, vector<string>> slotCourses; 
    for (auto& entry : courses) { 
        slotCourses[entry.second.color].push_back(entry.first); 
    } 
 
    for (auto& slot : slotCourses) { 
        int timeSlot = slot.first; 
        vector<string>& courseList = slot.second; 
 
        // Sort courses by number of students (descending) 
        sort(courseList.begin(), courseList.end(), 
             [&](string& a, string& b) { 
                 return courses[a].numStudents > courses[b].numStudents; 
             }); 
 
        // Reset room availability 
        for (auto& r : rooms) r.available = true; 
 
        for (auto& cname : courseList) { 
            bool assigned = false; 
            for (auto& r : rooms) { 
                if (r.available && r.capacity >= courses[cname].numStudents) { 
                    courses[cname].assignedRoom = r.name; 
                    r.available = false; 
                    assigned = true; 
                    break; 
                } 
            } 
            if (!assigned) 
                courses[cname].assignedRoom = "Not Available"; 
        } 
    } 
} 
 
// Display Final Timetable 
void displayTimetable(map<string, Course>& courses) { 
    cout << left << setw(10) << "Course" 
         << setw(15) << "Time Slot" 
         << setw(10) << "Room" << endl; 
    cout << "----------------------------------------\n"; 
    for (auto& entry : courses) { 
        cout << setw(10) << entry.first 
             << setw(15) << ("Slot " + to_string(entry.second.color + 1)) 
             << setw(10) << entry.second.assignedRoom << endl; 
    } 
} 
 
int main() { 
    // Step 1: Input Student-Course Data 
    map<string, vector<string>> studentCourses = { 
        {"S1", {"C1", "C2"}}, 
        {"S2", {"C2", "C3"}}, 
        {"S3", {"C1", "C3"}}, 
        {"S4", {"C4"}} 
    }; 
 
    // Step 2: Initialize Courses (Course Name → Students Count) 
    map<string, Course> courses = { 
        {"C1", {"C1", 90, -1, ""}}, 
        {"C2", {"C2", 70, -1, ""}}, 
        {"C3", {"C3", 50, -1, ""}}, 
        {"C4", {"C4", 40, -1, ""}} 
    }; 
 
    // Step 3: Initialize Rooms 
    vector<Room> rooms = { 
        {"R1", 100, true}, 
        {"R2", 80, true}, 
        {"R3", 50, true} 
    }; 
 
    // Step 4: Build Conflict Graph 
    map<string, vector<string>> adjList; 
    for (auto& entry : courses) { 
        adjList[entry.first] = {}; 
    } 
    buildConflictGraph(studentCourses, adjList); 
 
    // Step 5: Apply Graph Coloring 
    greedyColoring(adjList, courses); 
 
    // Step 6: Allocate Rooms 
    allocateRooms(courses, rooms); 
 
    // Step 7: Display Final Timetable 
    displayTimetable(courses); 
 
    return 0; 
}