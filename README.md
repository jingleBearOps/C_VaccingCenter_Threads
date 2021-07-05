# C_VaccingCenter_Threads
A vaccination centre consists of a registration desk (reg_desk) and from one to ten vaccination stations (vac_station). Members of the public arriving at the centre present themselves to the registration desk; the registration desk then enqueues such person (an instance of PersonInfo) at onto a single queue used for the whole centre (queue). That is, this single queue is an sequence of persons – in essence ordered by arrival at the registration desk – who are now queued up for a vaccination station to become available. (Multiple people arriving at the same time must be enqueued in the order in which they arrive at the registration desk.) Page 3 of 9 -- Tuesday, June 15, 2021 Meanwhile each of the vaccination stations dequeues a person from this single shared queue, and although each station is capable of vaccinating the person at the front, only one station will successfully remove the that person from the queue and perform their vaccination.

• The registration desk is a single POSIX thread, running the code in
reg_desk.
• Each vaccination station is its own POSIX thread, running their own
instance of the code in vac_station.

As an example, here are the contents of one of the simulation-events files (i.e.,
cases/01.txt):
1:3,60
2:5,70
3:6,50
4:8,30
5:8,40
6:9,50

./vaccine 3 cases/01.txt

Person 1: Arrived at time 3.
Person 1: Added to the queue.
Vaccine Station 1: START Person 1 Vaccination.
Person 2: Arrived at time 5.
Person 2: Added to the queue.
Vaccine Station 2: START Person 2 Vaccination.
Person 3: Arrived at time 6.
Person 3: Added to the queue.
Vaccine Station 3: START Person 3 Vaccination.
Person 4: Arrived at time 8.
Person 4: Added to the queue.
Person 5: Arrived at time 8.
Person 5: Added to the queue.
Person 6: Arrived at time 9.
Person 6: Added to the queue.
Vaccine Station 3: FINISH Person 3 Vaccination.
Vaccine Station 3: START Person 4 Vaccination.
Vaccine Station 1: FINISH Person 1 Vaccination.
Vaccine Station 1: START Person 5 Vaccination.
Vaccine Station 2: FINISH Person 2 Vaccination.
Vaccine Station 2: START Person 6 Vaccination.
Vaccine Station 3: FINISH Person 4 Vaccination.
Vaccine Station 1: FINISH Person 5 Vaccination.
Vaccine Station 2: FINISH Person 6 Vaccination.
