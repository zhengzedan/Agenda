#include "AgendaService.hpp"
#include <iostream>


AgendaService :: AgendaService(){
  startAgenda();
}

AgendaService :: ~AgendaService(){
  quitAgenda();
}

bool AgendaService :: userLogIn(const std::string &userName, const std::string &password){
  //用户名与密码是否对应
  auto filter = [userName, password](const User& user){
    return user.getName() == userName && user.getPassword() == password;
  };
  //用户名与密码不对应
  if(m_storage->queryUser(filter).size() == 0)
    return false;
  //用户名与密码对应
  return true;
}

bool AgendaService :: userRegister(const std::string &userName, const std::string &password,
                    const std::string &email, const std::string &phone){
  //用户是否已存在
  auto filter = [userName](const User& user){
    return user.getName() == userName;
  };
  //用户已存在
  if(m_storage->queryUser(filter).size() != 0)
    return false;
  //用户不存在时创建新用户
  User new_user(userName, password, email, phone);
  m_storage->createUser(new_user);
  return true;
}

bool AgendaService :: deleteUser(const std::string &userName, const std::string &password){
  //检查用户名与密码是否对应的函数
  auto filter = [userName, password](const User &user){
    return user.getName() == userName && user.getPassword() == password;
  };

  //该用户是否存在
  if(m_storage->deleteUser(filter) == 0)
    return false;

  //用户是否为会议发起人
  auto filter1 = [userName](const Meeting& meeting){
    return (meeting.getSponsor() == userName || meeting.getParticipator().size() == 0);
  };

  //用户是否为会议的参与者
  auto filter2 = [userName](const Meeting& meeting){
    return meeting.isParticipator(userName);
  };

  //移除会议中的该参与者
  auto switch2 = [userName](Meeting& meeting){
    meeting.removeParticipator(userName);
  };

  //删除有该用户参与的所有会议中该用户的记录，会议仍存在
  m_storage->updateMeeting(filter2, switch2);
  
  //删除该用户发起的所有会议，会议不存在
  m_storage->deleteMeeting(filter1);

  return true;
}

std::list<User> AgendaService :: listAllUsers(void) const{
  auto filter = [](const User& user){
    return true;
  };
  return m_storage->queryUser(filter);
}

bool AgendaService :: createMeeting(const std::string &userName, const std::string &title,
                     const std::string &startDate, const std::string &endDate,
                     const std::vector<std::string> &participator){
  //要创建的会议是否无效
  if(userName == "" || participator.size() == 0){
    //std::cout << "meeting is invalid" << std::endl;
    return false;
  }
    
  //判断会议的发起人和参与者是否已注册
  auto filter = [participator, userName](const User& user){
    for(auto iter = participator.begin(); iter != participator.end(); iter++){
      if((*iter) == user.getName() || user.getName() == userName)
        return true;
    }
    return false;
  };
  if(m_storage->queryUser(filter).size() != participator.size() + 1){
    //std::cout << "sponsor or participator hasn't register" << std::endl;
    return false;
  }
    
  //判断会议开始结束时间是否有效
  if(!Date::isValid(startDate) || !Date::isValid(endDate))
    return false;
  if(startDate >= endDate){
    //std::cout << "start time or end time is invalid" << std::endl;
    return false;
  }

  //判断日期格式是否正确
  if(startDate[4] != '-' || startDate[7] != '-' || startDate[10] != '/' || startDate[13] != ':')
    return false;
  if(endDate[4] != '-' || endDate[7] != '-' || endDate[10] != '/' || endDate[13] != ':')
    return false;
    
  //会议是否已存在
  auto filter1 = [title](const Meeting& meeting){
    if(meeting.getTitle() == title)
      return true;
    return false;
  };
  if(m_storage->queryMeeting(filter1).size() != 0){
    //std::cout << "meeting is insisting" << std::endl;
    return false;
  }
    
  //判断要创建的会议与发起人已有会议时间上是否冲突
  auto filter2 = [userName, startDate, endDate](const Meeting& meeting){
    if((meeting.getSponsor() == userName || meeting.isParticipator(userName)) && 
       (!(meeting.getStartDate() >= endDate || meeting.getEndDate() <= startDate)))
       return true;
    return false;
  };
  if(m_storage->queryMeeting(filter2).size() != 0){
    //std::cout << "sponsor time conflict" << std::endl;
    return false;
  }
    
  //判断要创建的会议与参与者已有会议时间上是否冲突
  auto filter3 = [participator, startDate, endDate](const Meeting& meeting){
    for(auto iter = participator.begin(); iter != participator.end(); iter++){
      if((*iter == meeting.getSponsor() || meeting.isParticipator(*iter)) &&
         (!(meeting.getStartDate() >= endDate || meeting.getEndDate() <= startDate))){
           return true;
         }    
    }
    return false;
  };
  if(m_storage->queryMeeting(filter3).size() != 0){
    //std::cout << "participator time conflict" << std::endl;
    return false;
  }
    

  //创建会议
  Meeting new_meeting(userName, participator, Date::stringToDate(startDate), Date::dateToString(endDate), title);
  // //###
  // std::cout << std::endl;
  // for(auto iter = participator.begin(); iter != participator.end(); iter++){
  //   std::cout << (*iter) << " ";
  // }
  // std::cout << std::endl;
  // std::cout << "new_meeting's detail below:" << std::endl;
  // std::cout << "new_meeting's participator's size is " << new_meeting.getParticipator().size() << std::endl;
  // for(auto iter = new_meeting.getParticipator().begin(); iter != new_meeting.getParticipator().end(); iter++){
  //   std::cout << (*iter) << " ";
  // }
  // std::cout << std::endl << "in service new_meeting's participators are:" << std::endl;
  // for(auto iter = new_meeting.getParticipator().begin(); iter != new_meeting.getParticipator().end(); iter++){
  //   std::cout << (*iter) << " ";
  // }
  // std::cout << std::endl;
  // //###
  m_storage->createMeeting(new_meeting);
  // //###
  // std::cout << std::endl << "in service createmeeting participators are : " << std::endl;
  // for(auto iter = participator.begin(); iter != participator.end(); iter++){
  //   std::cout << (*iter) << " ";
  // }
  // std::cout << std::endl;
  // //###
  return true;
}

bool AgendaService :: addMeetingParticipator(const std::string &userName,
                              const std::string &title,
                              const std::string &participator){
  //判断输入是否有效
  if(userName == "" || title == "" || participator == "")
    return false;
  
  //判断会议是否存在
  auto filter1 = [title, userName, participator](const Meeting& meeting){
    if(meeting.getTitle() == title && meeting.getSponsor() == userName)
      return true;
    return false;
  };
  if(m_storage->queryMeeting(filter1).size() == 0)
    return false;
  
  //判断新增加的参与者是否已注册
  auto filter2 = [participator](const User& user){
    if(user.getName() == participator)
      return true;
    return false;
  };
  if(m_storage->queryUser(filter2).size() == 0)
    return false;

  //判断该用户是否已经加入此次会议
  auto filter3 = [participator, title, userName](const Meeting& meeting){
    if(!meeting.isParticipator(participator) && meeting.getTitle() == title && meeting.getSponsor() == userName)
      return true; //未加入会议
    return false;
  };
  if(m_storage->queryMeeting(filter3).size() == 0)
    return false;

  //判断会议与要加入的参与者已有的会议时间上是否冲突
  std::list<Meeting> AllMeeting = listAllMeetings(participator);
  std::list<Meeting> targetMeeting = m_storage->queryMeeting(filter3);
  Date start = targetMeeting.begin()->getStartDate();
  Date end = targetMeeting.begin()->getEndDate();
  for(auto iter = AllMeeting.begin(); iter != AllMeeting.end(); iter++){
      if(!(iter->getStartDate() >= end || iter->getEndDate() <= start))
        return false;
  }

  //增加该参与者
  auto switcher = [participator](Meeting& meeting){
    meeting.addParticipator(participator);
  };
  if(m_storage->updateMeeting(filter3, switcher) == 0)
    return false;
  return true;
}

bool AgendaService :: removeMeetingParticipator(const std::string &userName,
                                 const std::string &title,
                                 const std::string &participator){
  //判断输入是否有效
  if(userName == "" || title == "" || participator == "")
    return false;

  //判断会议是否存在
  auto filter1 = [userName, title](const Meeting& meeting){
    if(meeting.getSponsor() == userName && meeting.getTitle() == title)
      return true;
    else 
      return false;
  };
  if(m_storage->queryMeeting(filter1).size() == 0)
    return false;
  
  //判断该参与者是否已注册
  auto filter2 = [participator](const User& user){
    if(user.getName() == participator)
      return true;
    else 
      return false;
  };
  if(m_storage->queryUser(filter2).size() == 0)
    return false;
  
  //判断该参与者是否没有加入会议
  auto filter3 = [participator, title, userName](const Meeting& meeting){
    if(meeting.isParticipator(participator) && meeting.getTitle() == title && meeting.getSponsor() == userName)
      return true;
    return false;
  };
  if(m_storage->queryMeeting(filter3).size() == 0)
    return false;
  
  
  //删除该参与者的函数
  auto switcher = [participator](Meeting& meeting){
    meeting.removeParticipator(participator);
  };

  //删除该参与者
  if(m_storage->updateMeeting(filter3, switcher) == 0)
    return false;
  
  //删除参与人数为0的会议
  auto filter4 = [](const Meeting& meeting){
    if(meeting.getParticipator().size() == 0)
      return true;
    return false;
  };
  m_storage->deleteMeeting(filter4);

  return true;
}

bool AgendaService :: quitMeeting(const std::string &userName, const std::string &title){
  //输入是否有效
  if(userName == "" || title == "")
    return false;

  //会议是否存在
  auto filter1 = [userName, title](const Meeting& meeting){
    if(meeting.getTitle() == title && meeting.isParticipator(userName) && meeting.getSponsor() != userName)
      return true;
    return false;
  };
  if(m_storage->queryMeeting(filter1).size() == 0)
    return false;

  //退出该会议
  auto switcher = [userName](Meeting& meeting){
    meeting.removeParticipator(userName);
  };
  m_storage->updateMeeting(filter1, switcher);

  //删除参与人数为0的会议
  auto filter2 = [](const Meeting& meeting){
    if(meeting.getParticipator().size() == 0)
      return true;
    return false;
  };
  m_storage->deleteMeeting(filter2);

  return true;
}

std::list<Meeting> AgendaService :: meetingQuery(const std::string &userName,
                                  const std::string &title) const{
  std::list<Meeting> result;

  //输出是否有效
  if(userName == "" || title == "")
    return result;
  
  //查询会议
  auto filter = [userName, title](const Meeting& meeting){
    if((meeting.getSponsor() == userName || meeting.isParticipator(userName)) && meeting.getTitle() == title)
      return true;
    return false;
  };
  result = m_storage->queryMeeting(filter);

  //返回查询结果
  return result;
}

std::list<Meeting> AgendaService :: meetingQuery(const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate) const{
  std::list<Meeting> result;

  //输入是否有效
  if(userName == "" || !Date::isValid(startDate) || !Date::isValid(endDate))
    return result;
  
  //查询会议
  auto filter = [userName, startDate, endDate](const Meeting& meeting){
    if((meeting.getSponsor() == userName || meeting.isParticipator(userName)) && 
            !(meeting.getEndDate() < startDate || meeting.getStartDate() > endDate))
      return true;
    return false;
  };
  result = m_storage->queryMeeting(filter);
  
  //返回查询结果
  return result;
}

std::list<Meeting> AgendaService :: listAllMeetings(const std::string &userName) const{
  auto filter = [userName](const Meeting& meeting){
    if(meeting.getSponsor() == userName || meeting.isParticipator(userName))
      return true;
    else 
      return false;
  };
  std::list<Meeting> result = m_storage->queryMeeting(filter);
  return result;
}

std::list<Meeting> AgendaService :: listAllSponsorMeetings(const std::string &userName) const{
  std::list<Meeting> result;

  //输入是否有效
  if(userName == "")
    return result;
  
  //查询
  auto filter = [userName](const Meeting& meeting){
    if(meeting.getSponsor() == userName)
      return true;
    return false;
  };
  result = m_storage->queryMeeting(filter);
  // //##
  // std::cout << std::endl;
  // for(auto iter = result.begin(); iter != result.end(); iter++){
  //   std::cout << "in service listAllSponsor meetings participators are : " << std::endl;
  //   for(auto iter1 = iter->getParticipator().begin(); iter1 != iter->getParticipator().end(); iter1++){
  //     std::cout << (*iter1) << " ";
  //   }
  //   std::cout << std::endl;
  // }
  // //##
  //返回查询结果
  return result;
}

std::list<Meeting> AgendaService :: listAllParticipateMeetings(
      const std::string &userName) const{
  std::list<Meeting> result;

  //输入是否有效
  if(userName == "")
    return result;
  
  //查询
  auto filter = [userName](const Meeting& meeting){
    if(meeting.isParticipator(userName) && meeting.getSponsor() != userName)
      return true;
    return false;
  };
  result = m_storage->queryMeeting(filter);

  //返回查询结果
  return result;
}

bool AgendaService :: deleteMeeting(const std::string &userName, const std::string &title){
  //输入是否有效
  if(userName == "" || title == "")
    return false;
  
  //查询该会议是否存在
  auto filter = [userName, title](const Meeting& meeting){
    if(meeting.getSponsor() == userName && meeting.getTitle() == title)
      return true;
    return false;
  };
  if(m_storage->queryMeeting(filter).size() == 0)
    return false;
  
  //删除会议
  m_storage->deleteMeeting(filter);
  return true;
}

bool AgendaService :: deleteAllMeetings(const std::string &userName){
  //输入是否有效
  if(userName == "")
    return false;
  
  //判断会议是否由username创建、删除会议
  auto filter = [userName](const Meeting& meeting){
    if(meeting.getSponsor() == userName)
      return true;
    return true;
  };

  //判断该用户是否创建过会议
  if(m_storage->queryMeeting(filter).size() == 0)
    return false;

  //删除会议
  m_storage->deleteMeeting(filter);
  return true;
}

void AgendaService :: startAgenda(void){
  m_storage = Storage::getInstance();
}

void AgendaService :: quitAgenda(void){
  m_storage->sync();
}