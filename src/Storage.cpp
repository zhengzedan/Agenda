#include "Storage.hpp"
#include "Path.hpp"
#include <iostream>
#include <fstream>

std::shared_ptr<Storage> Storage :: m_instance = nullptr;

Storage :: Storage(){
  m_dirty = false;
  readFromFile();
}

bool Storage :: readFromFile(void){
  std::ifstream userfile(Path::userPath);
  std::ifstream meetingfile(Path::meetingPath);
  if(!userfile.is_open() || !meetingfile.is_open())
    return false;
  //users
  std::string str;
  while(getline(userfile, str)){
    std::string name, password, email, phone;
    if(str.size() == 0)
      continue;
    std::vector<int> index;
    for(int i = 0; i < str.size(); i++){
      if(str[i] == '"'){
        index.push_back(i);
      }
    }
    for(int i = index[0] + 1; i < index[1]; i++)
      name = name + str[i];
    for(int i = index[2] + 1; i < index[3]; i++)
      password = password + str[i];
    for(int i = index[4] + 1; i < index[5]; i++)
      email = email + str[i];
    for(int i = index[6] + 1; i < index[7]; i++)
      phone = phone + str[i];
    User newUser(name, password, email, phone);
    m_userList.push_back(newUser);
  }
  userfile.close();

  str.clear();

  //meeting
  while(getline(meetingfile, str)){
    if(str.size() == 0)
      continue;
    std::vector<int> index;
    for(int i = 0; i < str.size(); i++){
      if(str[i] == '"')
        index.push_back(i);
    }
    std::string sponsor, participators, start_date, end_date, title;
    for(int i = index[0] + 1; i < index[1]; i++)
      sponsor = sponsor + str[i];
    for(int i = index[2] + 1; i < index[3]; i++)
      participators = participators + str[i];
    for(int i = index[4] + 1; i < index[5]; i++)
      start_date = start_date + str[i];
    for(int i = index[6] + 1; i < index[7]; i++)
      end_date = end_date + str[i]; 
    for(int i = index[8] + 1; i < index[9]; i++)
      title = title + str[i];

    std::vector<std::string> par;
    std::string strpar = "";
    for(int i = 0; i < participators.size(); i++){
      if(participators[i] != '&'){
        strpar.push_back(participators[i]);
      }else{
        par.push_back(strpar);
        strpar.clear();
      }
    }
    par.push_back(strpar);
    Meeting newMeeting(sponsor, par, Date(start_date), Date(end_date), title);
    m_meetingList.push_back(newMeeting);
  }
  meetingfile.close();
  m_dirty = false;

  return true;
}

bool Storage :: writeToFile(void){
  std::ofstream userfile(Path::userPath);
  std::ofstream meetingfile(Path::meetingPath);

  if(!userfile.is_open() || !meetingfile.is_open())
    return false;
  
  for(auto iter = m_userList.begin(); iter != m_userList.end(); iter++){
    userfile << '"' << (*iter).getName() << '"' << ',' << '"' << (*iter).getPassword() << '"' << ',' << '"' 
             << (*iter).getEmail() << '"' << ',' << '"' << (*iter).getPhone() << '"';
    userfile << "\n";
  }
  userfile.close();

  for(auto iter = m_meetingList.begin(); iter != m_meetingList.end(); iter++){
    meetingfile << '"' << (*iter).getSponsor() << '"' << ',' << '"';
    std::string participator;
    for(int i = 0; i < ((*iter).getParticipator()).size(); i++){
      participator = ((*iter).getParticipator())[i];
      meetingfile << participator;
      if((*iter).getParticipator().size() == 1)
        continue;
      if(i == (*iter).getParticipator().size() - 1){
        meetingfile << '"' << ',' << '"';
        continue;
      }
      meetingfile << '&';
    }
    meetingfile << (*iter).getStartDate().dateToString((*iter).getStartDate()) << '"' << ',' << '"'
                << (*iter).getEndDate().dateToString((*iter).getEndDate()) << '"' << ',' << '"'
                << (*iter).getTitle() << '"';
    meetingfile << "\n";
  }
  meetingfile.close();
  m_dirty = false;

  return true;
}

std::shared_ptr<Storage> Storage :: getInstance(void){
  if(m_instance == nullptr)
    m_instance = std::shared_ptr<Storage>(new Storage());
  return m_instance;
}

Storage :: ~Storage(){
  sync();
}

void Storage :: createUser(const User &t_user){
  m_userList.push_back(t_user);
  m_dirty = true;
}

std::list<User> Storage :: queryUser(std::function<bool(const User &)> filter) const{
  std::list<User> result;
  for(auto iter = m_userList.begin(); iter != m_userList.end(); iter++){
    if(filter(*iter))
      result.push_back(*iter);
  }
  return result;
}

int Storage :: updateUser(std::function<bool(const User &)> filter,
                 std::function<void(User &)> switcher){
  int count = 0;
  for(auto iter = m_userList.begin(); iter != m_userList.end(); iter++){
    if(filter(*iter)){
      switcher(*iter);
      count++;
    }
  }
  if(count)
    m_dirty = true;

  return count;
}

int Storage :: deleteUser(std::function<bool(const User &)> filter){
  int count = 0;
  for(auto iter = m_userList.begin(); iter != m_userList.end(); ){
    if(filter(*iter)){
      iter = m_userList.erase(iter);
      count ++;
    }
    else{
      iter++;
    }
  }
  if(count)
    m_dirty = true;

  return count;
}

void Storage :: createMeeting(const Meeting &t_meeting){
  // //###
  // std::cout << std::endl << "in storage create meeting t_meeting's participators are :" << std::endl;
  // for(auto iter = t_meeting.getParticipator().begin(); iter != t_meeting.getParticipator().end(); iter++){
  //   std::cout << (*iter) << " ";
  // }
  // std::cout << std::endl;
  // //###
  m_meetingList.push_back(t_meeting);
  m_dirty = true;
}

std::list<Meeting> Storage :: queryMeeting(
      std::function<bool(const Meeting &)> filter) const{
  std::list<Meeting> result;
  for(auto iter = m_meetingList.begin(); iter != m_meetingList.end(); iter++){
    if(filter(*iter)){
      result.push_back(*iter);
    }
  }
  // for(auto iter = result.begin(); iter != result.end(); iter++){
  //   std::cout << std::endl << "in storage querymeeting results are:" << std::endl;
  //   std::cout << Date::dateToString(iter->getStartDate()) << " " << Date::dateToString(iter->getEndDate());
  //   std::cout << std::endl;
  // }
  return result;
}

int Storage :: updateMeeting(std::function<bool(const Meeting &)> filter,
                    std::function<void(Meeting &)> switcher){
  int count = 0;
  for(auto iter = m_meetingList.begin(); iter != m_meetingList.end(); iter++){
    if(filter(*iter)){
      count++;
      switcher(*iter);
    }
  }
  if(count)
    m_dirty = true;
  
  return count;
}

int Storage :: deleteMeeting(std::function<bool(const Meeting &)> filter){
  int count = 0;
  for(auto iter = m_meetingList.begin(); iter != m_meetingList.end(); ){
    if(filter(*iter)){
      iter = m_meetingList.erase(iter);
      count++;
    }
    else{
      iter++;
    }
  }
  if(count){
    m_dirty = true;
  }

  return count;
}

bool Storage :: sync(void){
  m_dirty = false;
  return writeToFile();
}