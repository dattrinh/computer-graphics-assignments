# ME-C3100 Computer Graphics, Fall 2013
# Lehtinen / Peussa, Hölttä
#
# Assignment 0: Introduction

Student name: Trinh Ba Dat
Student number: 84475B
Hours spent on requirements (approx.): 8h  
Hours spent on extra credit (approx.): 8h

# First, some questions about where you come from and how you got started.
# Your answers in this section will be used to improve the course.
# They will not be judged or affect your points, but please answer all of them.
# Keep it short; answering shouldn't take you more than 5-10 minutes.

- What are you studying here at Aalto? (Department, major, minor...?) 
	TIK

- Which year of your studies is this? 
	This is 5th year of my studies

- Is ME-C3100 a mandatory course for you? 
	No

- Have you had something to do with graphics before? Other studies, personal interests, work? 
	No

- Do you remember basic linear algebra? Matrix and vector multiplication, cross product, that sort of thing? 
	No

- How is your overall programming experience? What language are you most comfortable with?
	Java, C# and C/C++

- Do you have some experience with these things? (If not, do you have experience with something similar such as C or Direct3D?)
	C++: Yes
	C++11: No
	OpenGL: Very basics

- Have you used a version control system such as Git, Mercurial or Subversion? Which ones?
	Yes, I do use Git.

- Did you go to the technology lecture?
	Not yet

- Did you go to exercise sessions?
	Not yet

- Did you work on the assignment using Aalto computers, your own computers, or both?
	I used my own computer.

# Which parts of the assignment did you complete? Mark them 'done'.
# You can also mark non-completed parts as 'attempted' if you spent a fair amount of
# effort on them. If you do, explain the work you did in the problems/bugs section
# and leave your 'attempt' code in place (commented out if necessary) so we can see it.

(Try to get everything done! Based on last year's data, virtually everyone who put in the work and did well in assignments 0 and 1 ended up finishing the course, and also reported a high level of satisfaction at the end of the course.)

                            opened this file (0p): done
                         R1 Moving an object (1p): done
R2 Generating a simple cone mesh and normals (3p): done
  R3 Converting mesh data for OpenGL viewing (3p): done
           R4 Loading a large mesh from file (3p): done

# Did you do any extra credit work?

(Describe what you did and, if there was a substantial amount of work involved, how you did it. Also describe how to use/activate your extra features, if they are interactive.)

	Version control
		- The assignment files are host on github and public link to repo is: https://github.com/dattrinh/computer-graphics-assignments.git
		- Log file is also included as required
		
	Implemented rotate and scale transforms
		- this demonstrates rotations only in one direction
		- keys x/y/z to rotate among axis x/y/z
		- keys "+"/"-" to scale in/out
		
	Added easy camera animation
		- press 'r' key to start or stop camera rotation
	
	Implemented easy viewpoint correction
		- This will correct viewpoint when window size is changed
	
	Added limited support for loading PLY file format and included a model for testing
		- Due to lack of time the limited function is provided with following features:
			- the loading does not handle properties. It assumes that properties are defined as x, y, z.
			- it supports only for triangle faces
			- it has not clever handler for property lists of faces. 
			- no calculation of normals provided
			
# Are there any known problems/bugs remaining in your code?

(Please provide a list of the problems. If possible, describe what you think the cause is, how you have attempted to diagnose or fix the problem, and how you would attempt to diagnose or fix it if you had more time or motivation. This is important: we are more likely to assign partial credit if you help us understand what's going on.)

	Some bugs in loading support for PLY
		- Overall, the object was loaded and displayed correctly. The sample object looks like tennis shoe, but there are some bugs in display of faces. I don't enough time to figure out what's wrong. 
	
# Did you collaborate with anyone in the class?
	No

(Did you help others? Did others help you? Let us know who you talked to, and what sort of help you gave or received.)

# Any other comments you'd like to share about the assignment or the course so far?

(Was the assignment too long? Too hard? Fun or boring? Did you learn something, or was it a total waste of time? Can we do something differently to help you learn? Please be brutally honest; we won't take it personally.)

	Very interesting subject. I would like to do all assignments. But due to lack of time some of them was be left. 
	
	It took me a lot of time to find out how calculation of matrix works and also with this framework. Instruction came to late in Piazza.
	