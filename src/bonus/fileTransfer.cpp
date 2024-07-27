/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileTransder.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 14:21:36 by oldault           #+#    #+#             */
/*   Updated: 2024/07/27 14:26:12 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void IRC::handleFileCommand(int cliSock, const std::string& message)
{
  std::istringstream lineStream(message);
  std::string target;
  std::string fileName;
  lineStream >> target >> fileName;
  std::string errMsg;

  if (!target.empty()) {
    target = target.substr(1);
  }

  if (target.empty() || fileName.empty()) {
    errMsg = getCommandPrefix(cliSock) + "PRIVMSG " + UNDL(target) + " : Usage: FILE <target> <filename>\r\n";
    send(cliSock, errMsg.c_str(), errMsg.length(), 0);
    return;
  }

  std::ifstream file(fileName.c_str(), std::ios::binary);
  if (!file) {
    errMsg = getCommandPrefix(cliSock) + "PRIVMSG " + UNDL(target) + FRED(" : Could not open file\r\n");
    send(cliSock, errMsg.c_str(), errMsg.length(), 0);
    return;
  }

  std::vector<char> fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  std::string directory = "./received_files/";
  mkdir(directory.c_str(), 0777);

  std::string filePath = directory + fileName;
  std::ofstream outFile(filePath.c_str(), std::ios::binary);
  if (!outFile) {
    errMsg = getCommandPrefix(cliSock) + "PRIVMSG " + UNDL(target) + " : Could not write file to directory\r\n";
    send(cliSock, errMsg.c_str(), errMsg.length(), 0);
    return;
  }

  outFile.write(&fileData[0], fileData.size());
  outFile.close();

  std::ostringstream convert;
  convert << cliSock;
  std::string cliSockStr = convert.str();

  std::string successMsg = getCommandPrefix(cliSock) + "PRIVMSG " + cliSockStr + " :File " + fileName + " stored successfully in " + filePath + "\r\n";
  send(cliSock, successMsg.c_str(), successMsg.length(), 0);

}