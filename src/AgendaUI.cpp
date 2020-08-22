#include "AgendaUI.hpp"
#include <iomanip>

AgendaUI :: AgendaUI(){
  startAgenda();
}

void AgendaUI :: OperationLoop(void){
  while(1){
    std::cout << std::endl
              << "---------------------------Agenda---------------------------" << std::endl;
    std::cout << "Action :" << std::endl;
    std::cout << "l    - log in Agenda by user name and password" << std::endl
              << "r    - register an Agenda account" << std::endl
              << "q    - quit Agenda" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << std::endl << "Agenda :~$ ";
    char operation;
    std::cin >> operation;
    switch(operation){
      case 'q' : quitAgenda(); break;
      case 'l' : userLogIn(); break;
      case 'r' : userRegister(); break; 
      default : break;
    }
    if(operation == 'q'){
      break;
    }
  }
}

void AgendaUI :: startAgenda(void){
  m_agendaService.startAgenda();
  OperationLoop();
}

std::string AgendaUI :: getOperation(){
  std::cout << std::endl << "Agenda@" << m_userName << " :~# ";
  std::string operation;
  std::cin >> operation;
  return operation;
}

bool AgendaUI :: executeOperation(const std::string &t_operation){
  if(t_operation == "o"){ userLogOut(); return false; } 
  else if(t_operation == "dc"){ deleteUser(); return false; }
  else if(t_operation == "lu"){ listAllUsers(); return true; }
  else if(t_operation == "cm"){ createMeeting(); return true; }
  else if(t_operation == "amp"){ addMeetingParticipator(); return true; }
  else if(t_operation == "rmp"){ removeMeetingParticipator(); return true; }
  else if(t_operation == "rqm"){ quitMeeting(); return true; }
  else if(t_operation == "la"){ listAllMeetings(); return true; }
  else if(t_operation == "las"){ listAllSponsorMeetings(); return true; }
  else if(t_operation == "lap"){ listAllParticipateMeetings(); return true; }
  else if(t_operation == "qm"){ queryMeetingByTitle(); return true; }
  else if(t_operation == "qt"){ queryMeetingByTimeInterval(); return true; }
  else if(t_operation == "dm"){ deleteMeetingByTitle(); return true; }
  else if(t_operation == "da"){ deleteAllMeetings(); return true; }
  else  return false;
}

void AgendaUI :: userLogIn(void){
  std::cout << std::endl
            << "[log in] [username] [password]" << std::endl
            << "[log in] ";
  std::string username, password;
  std::cin >> username >> password;
  if(m_agendaService.userLogIn(username, password)){
    m_userName = username, m_userPassword = password;
    std::cout << "[log in] succeed!" << std::endl << std::endl;
    std::cout << "---------------------------Agenda---------------------------" << std::endl
              << "o     - log out Agenda" << std::endl
              << "dc    - delete Agenda account" << std::endl
              << "lu    - list all Agenda user" << std::endl
              << "cm    - create a meeting" << std::endl
              << "amp   - add meeting participator" << std::endl
              << "rmp   - remove meeting participator" << std::endl
              << "rqm   - request to quit meeting" << std::endl
              << "la    - list all meetings" << std::endl
              << "las   - list all sponsor meetings" << std::endl
              << "lap   - list all participator meetings" << std::endl
              << "qm    - query meeting by title" << std::endl
              << "qt    - query meeting by time interval" << std::endl
              << "dm    - delete meeting by title" << std::endl
              << "da    - delete all meetings" << std::endl;
    std::cout << "------------------------------------------------------------";
    std::cout << std::endl;
    while(executeOperation(getOperation())){}
  }
  else{
    std::cout << "[log in] password error or user doesn't exist" << std::endl;
  }
}

void AgendaUI :: userRegister(void){
  std::cout << std::endl;
  std::cout << "[register] [username] [password] [email] [phone]" << std::endl;
  std::cout << "[register] ";
  std::string username, password, email, phone;
  std::cin >> username >> password >> email >> phone;
  if(m_agendaService.userRegister(username, password, email, phone))
    std::cout << "[register] succeed!" << std::endl;
  else
    std::cout << "[register] This username has been register!" << std::endl;
}

void AgendaUI :: userLogOut(void){
  m_userName = m_userPassword = "";
}

void AgendaUI :: quitAgenda(void){
  m_agendaService.quitAgenda();
}

void AgendaUI :: deleteUser(void){
  if(m_agendaService.deleteUser(m_userName, m_userPassword)){
    std::cout << std::endl  
              << "[delete agenda account] succeed!" << std::endl;
    m_userName = m_userPassword = "";
  }
  else
    std::cout << std::endl << "[delete agenda account] error!" << std::endl;
}

void AgendaUI :: listAllUsers(void){
  std::cout << "[list all users]" << std::endl << std::endl;
  int name_length = 6, email_length = 14, phone_length = 11;
  std::list<User> users = m_agendaService.listAllUsers();
  for(auto iter = users.begin(); iter != users.end(); iter++){
    name_length = iter->getName().size() > name_length ? iter->getName().size() : name_length;
    email_length = iter->getEmail().size() > email_length ? iter->getEmail().size() : email_length;
    phone_length = iter->getPhone().size() > phone_length ? iter->getPhone().size() : phone_length;
  }
  std::cout << std::setiosflags(std::ios_base::left) 
            << std::setw(name_length * 2) << "name" 
            << std::setw(email_length * 2) << "email"
            << "phone" 
            << std::endl;
  for(auto iter = users.begin(); iter != users.end(); iter++){
    std::cout << std::setiosflags(std::ios_base::left)
              << std::setw(name_length * 2) << iter->getName()
              << std::setw(email_length * 2) << iter->getEmail()
              << iter->getPhone()
              << std::endl;
  }
}

void AgendaUI :: createMeeting(void){
  std::cout << std::endl  
            << "[create meeting] [the number of participators]" << std::endl
            << "[create meeting] ";
  int participator_num;
  std::vector<std::string> participators;
  std::cin >> participator_num;
  for(int i = 1; i <= participator_num; i++){
    std::cout << "[create meeting] [please enter the participator " << i << " ]" << std::endl;
    std::cout << "[create meeting] ";
    std::string participator;
    std::cin >> participator;
    participators.push_back(participator);
  }

  std::cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << std::endl;
  std::cout << "[create meeting] ";
  std::string title, start_Date, end_Date;
  std::cin >> title >> start_Date >> end_Date;
  if(m_agendaService.createMeeting(m_userName, title, start_Date, end_Date, participators))
    std::cout << "[create meeting] succeed!" << std::endl;
  else
    std::cout << "[create meeting] error!" << std::endl;
}

void AgendaUI :: addMeetingParticipator(void){
  std::cout << std::endl
            << "[add participator] [meeting title] [participator username]" << std::endl
            << "[add participator] ";
  std::string title, participator;
  std::cin >> title >> participator;
  if(m_agendaService.addMeetingParticipator(m_userName, title, participator))
    std::cout << "[add participator] succeed!" << std::endl;
  else
    std::cout << "[add participator] error!" << std::endl;
}

void AgendaUI :: removeMeetingParticipator(void){
  std::cout << std::endl
            << "[remove participator] [meeting title] [participator name]" << std::endl
            << "[remove participator] ";
  std::string title, participator;
  std::cin >> title >> participator;
  if(m_agendaService.removeMeetingParticipator(m_userName, title, participator))
    std::cout << "[remove participator] succeed!" << std::endl;
  else
    std::cout << "[remove participator] error!" << std::endl;
}

void AgendaUI :: quitMeeting(void){
  std::cout << std::endl
            << "[quit meeting] [meeting title]" << std::endl  
            << "[quit meeting] ";
  std::string title;
  std::cin >> title;
  if(m_agendaService.quitMeeting(m_userName, title))
    std::cout << "[quit meeting] succeed" << std::endl;
  else
    std::cout << "[quit meeting] error" << std::endl;
}

void AgendaUI :: listAllMeetings(void){
  std::cout << "[list all meetings]" << std::endl << std::endl;
  std::list<Meeting> result = m_agendaService.listAllMeetings(m_userName);
  printMeetings(result);
}

void AgendaUI :: listAllSponsorMeetings(void){
  std::cout << std::endl
            << "[list all sponsor meetings]" << std::endl << std::endl;
  std::list<Meeting> result = m_agendaService.listAllSponsorMeetings(m_userName);
  printMeetings(result);
}

void AgendaUI :: listAllParticipateMeetings(void){
  std::cout << std::endl
            << "[list all participator meetings]" << std::endl << std::endl;
  std::list<Meeting> result = m_agendaService.listAllParticipateMeetings(m_userName);
  printMeetings(result);
}

void AgendaUI :: queryMeetingByTitle(void){
  std::cout << std::endl  
            << "[query meeting] [title]" << std::endl
            << "[query meeting] ";
  std::string title;
  std::cin >> title;
  std::list<Meeting> result;
  result = m_agendaService.meetingQuery(m_userName, title);
  printMeetings(result);
}

void AgendaUI :: queryMeetingByTimeInterval(void){
  std::cout << std::endl    
            << "[query meetings] [start time(yyyy-mmmm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << std::endl
            << "[query meetings] ";
  std::string start_date, end_date;
  std::cin >> start_date >> end_date;
  std::cout << "[query meetings]" << std::endl << std::endl;
  std::list<Meeting> result = m_agendaService.meetingQuery(m_userName, start_date, (end_date));
  printMeetings(result); 
}

void AgendaUI :: deleteMeetingByTitle(void){
  std::cout << std::endl
            << "[delete meeting] [title]" << std::endl
            << "[delete meeting] ";
  std::string meeting;
  std::cin >> meeting;
  if(m_agendaService.deleteMeeting(m_userName, meeting)){
    std::cout << "[delete meeting] succeed!" << std::endl;
  }else{
    std::cout << "[error] delete meeting fail!" << std::endl;
  }
}

void AgendaUI :: deleteAllMeetings(void){
  m_agendaService.deleteAllMeetings(m_userName);
  std::cout << std::endl
            << "[delete all meeting] succeeed!" << std::endl;
}

void AgendaUI :: printMeetings(const std::list<Meeting> &t_meetings){
  int title_length = 5, sponsor_length = 7, date_length = 16, participator_length = 12;
  for(auto iter = t_meetings.begin(); iter != t_meetings.end(); iter++){
    title_length = iter->getTitle().size() > title_length ? iter->getTitle().size() : title_length;
    sponsor_length = iter->getSponsor().size() > sponsor_length ? iter->getSponsor().size() : sponsor_length;
  }
  std::cout << std::setiosflags(std::ios_base::left) 
            << std::setw(title_length * 2) << "title"
            << std::setw(sponsor_length * 2) << "sponsor" 
            << std::setw(date_length * 2) << "start time" 
            << std::setw(date_length * 2) << "end time"
            << "participators" << std::endl;

  for(auto iter = t_meetings.begin(); iter != t_meetings.end(); iter++){
    std::cout << std::setiosflags(std::ios_base::left) 
              << std::setw(title_length * 2) << iter->getTitle()
              << std::setw(sponsor_length * 2) << iter->getSponsor()
              << std::setw(date_length * 2) << Date::dateToString(iter->getStartDate())
              << std::setw(date_length * 2) << Date::dateToString(iter->getEndDate());
    auto participators = iter->getParticipator();
    for(int i = 0; i < participators.size(); i++){
      std::cout << participators[i];
      if(i != participators.size() - 1)
        std::cout << ",";
    }
    std::cout << std::endl;
  }
}