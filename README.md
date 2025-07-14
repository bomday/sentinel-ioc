# Sentinel IOC

🔗 [GitHub Pages](https://bomday.github.io/sentinel-ioc/)

> A C++ project for analyzing Indicators of Compromise (IOCs) using Object-Oriented Programming principles.

## Description

Sentinel IOC simulates an IOC analyzer (malicious IPs, URLs, and hashes), enabling users to register, view, update, and delete this information. It's a practical introduction to Object-Oriented Programming (OOP) in C++.

## Technologies Used

- **Language:** C++
- **OOP Concepts:** Inheritance, Polymorphism, Encapsulation & Abstraction
- **File Handling:** CSV (simulating database)
- **Tools:** Git & GitHub
- **Documentation:** GitHub Pages
- **Other:** Pointers and References

## Features

- [x] Register IOCs (IP, URL, Hash)
- [x] List and search IOCs
- [x] Update and delete IOCs
- [x] Simulated threat analysis
- [x] Command-line interface
- [ ] Graphical User Interface (optional, under development)

## Project Structure

```
📁 sentinel-ioc/
 ┣ 📁 googletest                   >> Unit tests
 ┣ 📁 src/
 ┃ ┣ 📁 CLI                 
 ┃ ┃ ┣ 📄 cli.hpp                  >> CLI interface class
 ┃ ┃ ┗ 📄 cli.cpp
 ┃ ┣ 📁 data/
 ┃ ┃ ┗ 📄 iocs.csv                 >> Simulated database (CSV format)
 ┃ ┣ 📁 fileManager                 
 ┃ ┃ ┣ 📄 fileManager.hpp          >> File read/write manager
 ┃ ┃ ┗ 📄 fileManager.cpp
 ┃ ┣ 📁 indicator                 
 ┃ ┃ ┣ 📄 indicator.hpp            >> Abstract base class for IOCs
 ┃ ┃ ┗ 📄 indicator.cpp              
 ┃ ┣ 📁 indicatorManager
 ┃ ┃ ┣ 📄 indicatorManager.hpp     >> Manages IOC CRUD operations
 ┃ ┃ ┗ 📄 indicatorManager.cpp
 ┃ ┣ 📁 maliciousHash
 ┃ ┃ ┣ 📄 maliciousHash.hpp        >> Derived class for malicious hash
 ┃ ┃ ┗ 📄 maliciousHash.cpp
 ┃ ┣ 📁 maliciousIP
 ┃ ┃ ┣ 📄 maliciousIP.hpp          >> Derived class for malicious IP
 ┃ ┃ ┗ 📄 maliciousIP.cpp
 ┃ ┣ 📁 maliciousURL
 ┃ ┃ ┣ 📄 maliciousURL.hpp         >> Derived class for malicious URL
 ┃ ┃ ┗ 📄 maliciousURL.cpp
 ┃ ┣ 📁 utils
 ┃ ┃ ┣ 📄 utils.hpp                >> Utility functions (e.g., menu, timestamps)
 ┃ ┃ ┗ 📄 utils.cpp
 ┃ ┣ 📄 sentinel.exe               >> Main executable
 ┃ ┣ 📄 sentinel_tests.exe         >> Unit tests executable   
 ┃ ┗ 📄 main.cpp                   >> Application entry point
 ┣ 📁 public/
 ┃ ┣ 📁 assets                     >> Project images
 ┃ ┣ 📄 sentinel-ioc.puml          >> UML diagram 
 ┃ ┗ 📄 report.md                  >> Project report
 ┃ ┗ 📄 report.pdf                 >> Project report
 ┗ 📄 README_EN.md
```

## OOP Concepts Demonstrated

- **Classes & Objects:** Used to define and instantiate IOCs and their manager.
- **Inheritance:** `MaliciousIP`, `MaliciousURL`, and `MaliciousHash` inherit from the base class `Indicator`.
- **Polymorphism:** A single `Indicator*` pointer can refer to any derived IOC type.
- **Encapsulation:** Controlled access via `private`, `protected`, and `public` members.
- **Abstraction:** The `Indicator` class acts as an abstract interface for different types of indicators. The `CLI` only interacts with the `IndicatorManager`'s public interface, without knowing the internal details of storing, searching or generating IDs.

## How to Run on CLI

### Clone the Repository

```bash
git clone https://github.com/bomday/sentinel-ioc.git
```

### Requirements

- g++ version 7.0 or later
- Compile and run from the `/src` directory

### Compile the Program

```bash
g++ -g -std=c++17 -o sentinel.exe main.cpp cli/cli.cpp utils/utils.cpp indicator/indicator.cpp maliciousIP/maliciousIP.cpp maliciousURL/maliciousURL.cpp maliciousHash/maliciousHash.cpp indicatorManager/indicatorManager.cpp fileManager/fileManager.cpp -I. -Icli -Iutils -Iindicator -ImaliciousIP -ImaliciousURL -ImaliciousHash -IindicatorManager -IfileManager
```

### Run the Program

```bash
./sentinel.exe
```

## Running Unit Tests

See the [Test README](https://github.com/bomday/sentinel-ioc/blob/main/googletest/README.md)

## Running the Graphical User Interface (GUI)
See the [GUI README](https://github.com/bomday/sentinel-ioc/blob/main-GUI/src/gui/README.md)

## Video Demo

🔗 Understand our structure: [YouTube](https://youtu.be/9lCIjkyFWyg)

🔗 Watch our Demo: [YouTube](https://www.youtube.com/watch?v=5k7H-dmm7U4)

> Interface finalized after first delivery

## Images

![Sentinel IOC Interface](/public/assets/Image1.jpg)

![Sentinel IOC Interface](/public/assets/Image2.jpg)

![Sentinel IOC Interface](/public/assets/Image3.jpg)

![Sentinel IOC Interface](/public/assets/Image4.jpg)

## Project Report

See the [Report](https://github.com/bomday/sentinel-ioc/blob/main/public/report.pdf)

## Team

| Name            | Course               |
|-----------------|------------------------|
| Beatriz Helena  | Information Systems    |
| Dayane Lima     | Information Systems    |
| Maria Antônia   | Information Systems    |
| Rafael Teles   | Information Systems    |
| Williams Andrade| Information Systems    |
