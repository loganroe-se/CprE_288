# Course Number
CprE 288 - Embedded Systems I: Introduction

# Class Overview
CprE 288 was a class primarily based around a CyBot (i.e. a robot that performed and was controlled similarly to an automated robot vacuum). The main idea was to use numerous labs to build up to a final project where the bot could be controlled, blindly, via either a GUI or fully automated, or even both. The class focused on UART, Embedded Software, and learning C code, as this is the first class in the CprE curriculum for C/C++. Importantly, all of labs were conducted by groups of two, in which each person had a direct partner, and the final project was a culmination of all of the labs and the joining of two groups of two, equaling four total people.

# Year Taken
Sophomore Year - Semester 2

# Grade Received
* Overall Class Grade: A
* Final Project Grade: A

# Size of Group
Number of People: 4
* Myself, Logan Roe
* My partner, Daryl Kay
* Partners that joined us:
  * Gavin Rich
  * Matthew Brammer

# Goal of the Project
Control the CyBot, via either remote, blind control of the CyBot using a GUI or an automated algorithm with no human intervention. If desired, both could be implemented. The primary goal was to navigate the CyBot through a field, outlined my white tape, which could not be exceeded as they marked the field boundaries, detect objects using IR & Ping sensors for distanced, taller objects and detect shorter objects with bump sensors. Further, a hole was present as well which was detected by sensors on the bottom of the bot. The end goal was to arrive at a square grid, using four skinny "pillars" to mark it.

We would be penalized if any taller objects were ran into even a single time or if any of the shorter objects were ran into a second time. Further, we could not fall in the hole or exceed boundaries without receiving significant penalties. If we exceeded the provided time limit, we would also receive penalties for how much time we used past the limit.

# Notes
The majority of the final project's code may be found in the [288-mission/Final Project](https://github.com/loganroe-se/CprE_288/tree/1d10bfa3dd542ef13959e4a639ffdfaf043a7eac/288-mission/Final%20Project) folder.

**Disclaimer: Due to the nature of the project and our general inexperience with projects, especially in groups, at this point, the code is not well organized or commented. Due to my lack of experience with programming at this point, these files are not written to the standard that I hold myself to now.
