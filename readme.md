# Student & Faculty Management System

A **console-based C++ application** for managing academic records of students and faculty members in an educational institution.

The program is fully contained in **`main.cpp`** (the source you provided) and uses only the C++ Standard Library – no external dependencies.

---

## Features

### Student Management
| Feature | Description |
|---------|-------------|
| **Add Student** | Register a student with Roll No, Name, Age |
| **Add Semester** | Up to **8** semesters per student (auto-numbered) |
| **Add Subject** | Subject name, marks (0-100), credits (default **4**) |
| **Backlog Detection** | Marks **< 40** are automatically flagged |
| **Clear Backlog** | Update marks/credits after supplementary exam |
| **SGPA Calculation** | `GP = floor(marks/10)` if marks ≥ 40, else **0**  <br> `SGPA = Σ(GP × Credits) / Σ(Credits)` |
| **CGPA Calculation** | Average of all semester SGPA values |
| **Academic Summary** | Per-student or **all students** view: <br> • Semester-wise subjects, marks, credits <br> • SGPA, CGPA, total backlogs |
| **Input Validation** | Marks range, duplicate subjects, semester existence |

### Faculty Management
| Feature | Description |
|---------|-------------|
| **Add Faculty** | Name, Age, Qualification, Grade, Experience, Research Area, Department |
| **Auto-generated ID** | `first3(name) + age + first3(department)` (e.g., `Ram25CSE`) |
| **Promote Faculty** | Update grade |
| **Add Qualification** | Append new qualification |
| **Change Research Area** | Update research field |
| **Faculty Details** | Individual or **all faculty** view |

### General
- **Interactive CLI menu** (11 options)
- **Object-oriented design** with inheritance (`Person → Student/Faculty`)
- **STL usage** – `vector`, `map`, `algorithm`, lambdas, `emplace_back`
- **Error messages** for invalid inputs / not-found entities

---

## Project Structure (single file)
