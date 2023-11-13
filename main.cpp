#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <bits/stdc++.h> 
#include <string.h>
#include <iomanip>
#include <termios.h>
#include <stdio.h>
#include <cstdio>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/ioctl.h>
#include <fstream>
#include <conio.h>

using namespace std;
string mode="NORMAL MODE";
long long fileCount=0;
int xcurr=1,ycurr=1;
vector<string> listOfFiles;
stack<string> navStack;
stack<string> navStackForward;
string currentPath;
string basePath="/home/";
int currRow;
int c=0;
int c1=0;
int winoffset=7;

//Function to get current terminal size
int getWinRow()
{
	struct winsize winsize;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);
	int winrow = winsize.ws_row;
	int wincol = winsize.ws_col;
	return winrow;
}

//Function to place cursor at given coordinates
void posCursor(int r,int c)
{
	printf("%c[%d;%dH", 27, r, c);
}

//Fucntion to get file/directory permissions
string checkPermissions(struct stat filesInfo)
{
	string perm="";
	if((S_ISDIR(filesInfo.st_mode)))
	{
		perm+="d";
	}
	else
	{
		perm+="-";
	}
	if((filesInfo.st_mode & S_IRUSR)) perm+="r"; else perm+="-"; // read permission
	if((filesInfo.st_mode & S_IWUSR)) perm+="w"; else perm+="-"; // write permission
	if((filesInfo.st_mode & S_IXUSR)) perm+="x"; else perm+="-"; // execute permission
	if((filesInfo.st_mode & S_IRGRP)) perm+="r"; else perm+="-"; 
	if((filesInfo.st_mode & S_IWGRP)) perm+="w"; else perm+="-";
	if((filesInfo.st_mode & S_IXGRP)) perm+="x"; else perm+="-";
	if((filesInfo.st_mode & S_IROTH)) perm+="r"; else perm+="-";
	if((filesInfo.st_mode & S_IWOTH)) perm+="w"; else perm+="-";
	if((filesInfo.st_mode & S_IXOTH)) perm+="x"; else perm+="-";
	
	return perm;
}

//Function to get path without dots
string getRealPath(string path)
{
	char* ans = realpath(path.c_str(),NULL);
	string anspath = ans;
	return anspath;
}

//Function to convert filesize to readable form
string getFileSize(long long fileSize)
{
	string ans="";
	if(fileSize<=1024)
	{
		return to_string(fileSize)+"B";
	}
	fileSize = fileSize/1024;
	if(fileSize<=1024)
	{
		ans+=to_string(fileSize)+"KB";
	}else if(fileSize<=(1024*1024))
	{
		ans+=to_string(fileSize/1024)+"MB";
	}else if(fileSize<=(1024*1024*1024))
	{
		ans+=to_string((fileSize/1024)/1024)+"GB";
	}
	return ans;
}

//Function to get file name from path
string getFileName(string path)
{
	string fileName="";
	for(int i=path.length()-1;i>=0;i--)
	{
		if(path[i]=='/')
			break;
		else fileName+=path[i];
	}
	reverse(fileName.begin(),fileName.end());
	return fileName;
	
}

//Function to get folder name from path
string getFolderName(string path)
{
	string folderName="";
	int flag=0;
	for(int i=path.length()-1;i>=0;i--)
	{
		if(path[i]=='/' && flag==0)
		{
			flag=1;
		}
		else if(path[i]=='/' && flag==1)
			break;
		else folderName+=path[i];
	}
	reverse(folderName.begin(),folderName.end());
	return folderName;
	
}

//Function to get parent directory of given path
string getParentDir(string path)
{
	if(path=="" || path=="/")
		return "/";
	if(path[path.length()-1]!='/')
		path = path+'/';
	path = getRealPath(path)+'/';
	string pDir="";
	int slashCount=0;
	int startnow=0;
	for(int i=path.length()-1;i>=0;i--)
	{
		if(path[i]=='/' && slashCount<2)
		{
			slashCount++;
		}
		
		if(slashCount==2 && startnow==0)
		{
			startnow=1;
		}
		else if(startnow!=0)
		{
			pDir+=path[i];
		}
	}
	reverse(pDir.begin(),pDir.end());
	pDir+='/';
	return pDir;
}

//Function to parse path and convert to our useable form
string getFullPath(string path)
{
	string ans="";
	if(path[0]=='~')
	{
		ans=basePath+path.substr(2,path.length()-1);
	}
	else if(path[0]=='.' && path.length()==1)
	{
		ans=currentPath;
	}
	else if(path[0]=='.' && path[1]!='.')
	{
		ans=currentPath+path.substr(2,path.length()-1);
	}
	else if(path[0]=='.' && path[1]=='.' && path.length()<=2)
	{
		ans=getParentDir(currentPath);
	}
	else if(path[0]=='.' && path[1]=='.' && path.length()>2)
	{
		
		ans=getParentDir(currentPath) + path.substr(3,path.length()-1); 
	}
	else if(path[0]=='/')
	{
		ans=path;
	}
	else
	{
		ans=currentPath+path;
	}
	struct stat filesInfo;
	stat(ans.c_str(),&filesInfo);	
	if(S_ISDIR(filesInfo.st_mode) && ans[ans.length()-1]!='/')
	{
		ans+='/';
	}
	
	return ans;
}

//Function to truncate large file names
string getTruc(string justFileName, int width)
{
	if(justFileName.length()>width-2)
	{
		justFileName = justFileName.substr(0,width-5)+"...";
	}
	return justFileName;
}

//Function to display files
void display(string path)
{
	
	char separator =' ';
	int nameWidth=25;
	int permWidth=15;
	int ownerWidth=15;
	int sizeWidth=10;
	int timeWidth=25;
	struct stat filesInfo;
	string perm="";
	string justFileName = getFileName(path);
	if(path=="printcurrentdir")
	{
		cout<<endl;
		cout << "\033[1;32mCurrent Directory:\033[0m"<<getRealPath(currentPath)<<endl;
	}
	else if(path=="---")// to print mode
	{
		cout<<"************"<<mode<<"************"<<endl;
	}
	else if(stat(path.c_str(),&filesInfo)==0)
	{
		perm=checkPermissions(filesInfo);
		cout<<left<<setw(nameWidth)<<setfill(separator)<<getTruc(justFileName,nameWidth);
		cout<<left<<setw(permWidth)<<setfill(separator)<<perm;
		cout<<left<<setw(ownerWidth)<<setfill(separator)<<getpwuid(filesInfo.st_uid)->pw_name;
		cout<<left<<setw(ownerWidth)<<setfill(separator)<<getgrgid(filesInfo.st_gid)->gr_name;
		cout<<left<<setw(sizeWidth)<<setfill(separator)<<getFileSize(filesInfo.st_size);
		cout<<left<<setw(timeWidth)<<setfill(separator)<<ctime(&filesInfo.st_mtime);
		
	}else{
		perror(path.c_str());
	}
	
}

//Function to get type of command
string getCommandType(string enteredCommand)
{
	string ans = "";
	for(int i = 0; i < enteredCommand.length(); i++)
	{
		if(enteredCommand[i] == ' ')
			break;
		else
			ans += enteredCommand[i];
	}
	return ans;
}

//Function to create new file
void createNewFile(string path,string newfilename)
{
	DIR *dr;
	struct dirent *ent;
	if(path=="")
		path=currentPath;
	// Since we are already checking whether file exists or not, these flags create a new file
	int fileStatus = open((path+newfilename).c_str(),O_RDWR | O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);
	if(fileStatus==-1)
	{
		perror(path.c_str());
	}
}
//Function to create new directory
void createNewDir(string path,string newdirname)
{
	DIR *dr;
	struct dirent *ent;
	if(path=="")
		path=currentPath;
	int dirStatus = mkdir((path+newdirname).c_str(),S_IRWXU|S_IRWXG|S_IRWXO);
	if(dirStatus==-1)
	{
		perror(path.c_str());
	}
}

//Function to parse input
string arg1(string enteredCommand)
{
	string ans="";
	int flag=0;
	for(int i=0;i<enteredCommand.length();i++)
	{
		if(enteredCommand[i]==' ')
		{
			flag++;
		}
		else if(flag==1)
		{
			ans+=enteredCommand[i];
		}
		
	}
	return ans;
}

//Function to parse input
string arg2(string enteredCommand)
{	
	string ans="";
	for(int i=enteredCommand.length()-1;i>=0;i--)
	{
		if(enteredCommand[i]!=' ')
		{
			ans+=enteredCommand[i];
		}else break;
	}
	reverse(ans.begin(),ans.end());
	return ans;
}

//Function to get all source files, directories from input
vector<string> getAllSrcs(string enteredCommand)
{
	vector<string> srcs;
	string ans="";
	for(int i=0;i<enteredCommand.length();i++)
	{
		if(enteredCommand[i]==' ')
		{
			srcs.push_back(ans);
			ans="";
		}else
		{
			ans+=enteredCommand[i];
		}
	}
	srcs.erase(srcs.begin());
	return srcs;
}

//Function to copy single file
void copyFile(string srcfile,string desdir)
{	
	string row;
	ifstream srcfilestream;
	ofstream desfilestream;
	// open src, open dest file, copy each line
	srcfilestream.open(srcfile.c_str(),ios::in);
	srcfile = getFileName(srcfile);
	desfilestream.open((desdir+"/"+srcfile).c_str(),ios::out);
	while(getline(srcfilestream,row))
	{
		// copying 
		desfilestream<<row<<"\n";
	}
	
	srcfilestream.close();
	desfilestream.close();
}

//Function to delete single file
void deleteFile(string fileToDelete)
{
	// remove is present in stdio.h, return non zero if fails
	if(remove(fileToDelete.c_str()) != 0)
	{
		perror("Error in deleting file");
	}
}

//Function to get all files from given directory
vector<string> getAllFiles(string dirname)
{
	vector<string> ans;
	DIR* dir;
	struct dirent *d;
	
	dir = opendir(dirname.c_str());
	if(dir == NULL)
		return ans;
	if(dir != NULL){
		while((d = readdir(dir)) != NULL)
		{
			ans.push_back(dirname+d->d_name);
		}
	}
	return ans;
}

//Function to recursively search in directory
bool searchInDir(string dirname, string toSearch)
{
	vector<string> fileList = getAllFiles(dirname);
	for(int i = 0; i < fileList.size(); i++)
	{
		struct stat filesInfo;
		stat(fileList[i].c_str(),&filesInfo);
		// is regular file
		if(S_ISREG(filesInfo.st_mode))
		{
			
			string justFileName = getFileName(fileList[i]);
			if(justFileName == toSearch)
			{
				return true;
			}else continue;
		}
		else if(getFileName(fileList[i]) == "." || getFileName(fileList[i]) == "..")
		{
			continue;
		}
		else // if directory
		{
			// if directory was searched
			string justFileName = getFileName(fileList[i]);
			if(justFileName == toSearch)
			{
				return true;
			}
			// search inside directory
			bool ansreturned =  searchInDir(fileList[i]+"/",toSearch);
			if(ansreturned==true)
				return true;			
		}
	}
	return false;
}

//Function to copy directory
void copyingFunction(string srcpath,string desdir)
{	
	vector<string> fileList = getAllFiles(srcpath);
	createNewDir(desdir,getFolderName(srcpath));
	desdir = desdir+getFolderName(srcpath);
	for(int i = 0; i < fileList.size(); i++)
	{
		struct stat filesInfo;
		stat(fileList[i].c_str(),&filesInfo);	
		// S_ISREG test its regular file or not
		if(S_ISREG(filesInfo.st_mode))
		{
			copyFile(fileList[i],desdir);
		}
		else if(getFileName(fileList[i])=="." || getFileName(fileList[i])=="..")
		{
			continue;
		}
		else
		{	
			copyingFunction(fileList[i]+"/",desdir+"/");		
		}
	}
}

//Function to delete whole directory
void deletingFunction(string srcpath)
{	
	vector<string> fileList = getAllFiles(srcpath);
	for(int i = 0; i < fileList.size(); i++)
	{
		struct stat filesInfo;
		stat(fileList[i].c_str(),&filesInfo);	
		// Delete file
		if(S_ISREG(filesInfo.st_mode))
		{
			deleteFile(fileList[i]);
		}
		else if(getFileName(fileList[i])=="." || getFileName(fileList[i])=="..")
		{
			continue;
		}
		else
		{
			// Recursively delete files
			deletingFunction(fileList[i]+"/");		
		}
	}
	if(rmdir(srcpath.c_str())!=0)
	{
		perror("Error in deleting");
		
	}
}

//Function to get parent directory of given file
string getParentOfFile(string path)
{
	string ans="";
	int flag=0;
	for(int i = path.length() - 1; i >= 0; i--)
	{
		if(path[i] == '/' && flag==0)
			flag++;
		else if(flag >= 1)
			ans += path[i];
	}
	reverse(ans.begin(),ans.end());
	ans += '/';
	return ans;
}

//Function to search in just immediate directory
bool findInCurrent(string dirname,string fdname,string fileOrDir)
{
	vector<string> filesAndDirInCurrent;
	DIR* dir;
	struct dirent *d;
	
	dir = opendir(dirname.c_str());
	if(dir != NULL){
		while((d = readdir(dir)) != NULL)
		{
			filesAndDirInCurrent.push_back(dirname+d->d_name);
		}
	}
	closedir(dir);
	for(int i = 0; i < filesAndDirInCurrent.size(); i++)
	{
		struct stat filesInfo;
		stat(filesAndDirInCurrent[i].c_str(), &filesInfo);
		
		if(S_ISREG(filesInfo.st_mode) && fileOrDir == "file")
		{
			if(getFileName(filesAndDirInCurrent[i]) == getFileName(fdname))
			{
				return true;
			}
				
		}
		else if(S_ISDIR(filesInfo.st_mode) && fileOrDir == "dir")
		{
			if(getFolderName(filesAndDirInCurrent[i]+"/") == getFolderName(fdname))
			{
				return true;
			}
		}
	}
	return false;

}

//Function to check if path is valid
bool validPath(string path)
{
	struct stat filesInfo;
	stat(path.c_str(),&filesInfo);	
	if(S_ISDIR(filesInfo.st_mode))
	{
		return true;
	}
	return false;
}

//Function to make a list of all files 
vector<string> listAllFiles(string dirname)
{
	cout << "\033[2J\033[1;1H";
	xcurr=1;
	ycurr=1;
	posCursor(xcurr,ycurr);
	fileCount=0;
	DIR* dir;
	struct dirent *d;
	
	dir = opendir(dirname.c_str());
	if(dir == NULL)
		return listOfFiles;
	if(dir != NULL){
		while((d = readdir(dir)) != NULL)
		{
			listOfFiles.push_back(dirname+d->d_name);
		}
	}
	closedir(dir);
	sort(listOfFiles.begin(),listOfFiles.end());
	fileCount = listOfFiles.size();
	int maxprintlimit = listOfFiles.size() < getWinRow() - winoffset ? listOfFiles.size(): getWinRow() - winoffset;
	for(int i = 0; i < maxprintlimit; i++)
	{
		display(listOfFiles[i]);
	}
	posCursor(getWinRow() - winoffset + 1, ycurr);
	display("printcurrentdir");
	display("---");
	
	c1 = maxprintlimit - 1;
	posCursor(1,1);
	currRow = getWinRow();
	return listOfFiles;
	
}

//Function to clear command mode area
void clearCommandMode()
{
	listOfFiles.clear();
	c = 0;
	listAllFiles(currentPath);
	xcurr = getWinRow() - winoffset + 4;
	posCursor(xcurr,ycurr);
}

int main()
{
	
	uid_t uid = geteuid(); //to get username of system
	string useridpc = getpwuid(uid)->pw_name; // Gives username of user based on uid, getpwuid() gives user details
	string path = "/home/"+ useridpc +"/Desktop/";
	currentPath = path;
	basePath += useridpc + "/";
	struct termios term; // terminal interface
    tcgetattr(fileno(stdin), &term); // get terminal attributes
    // Switching OFF the typing on terminal
    term.c_lflag &= ~ECHO;
    term.c_lflag &= ~ICANON;
    tcsetattr(fileno(stdin), 0, &term); // set terminal attributes
    navStack.push(currentPath); // keeping track of path for navigation (forward, backward)
	listAllFiles(currentPath.c_str());
	char ch;
	while(1)
	{	
		if(getWinRow()!=currRow) //manage vertical resizing
		{
			fileCount = 0;
			listOfFiles.clear();
			c = 0;
			listAllFiles(currentPath.c_str());
			currRow = getWinRow();
		}
		if(kbhit()) // keyboard hit
		{
		
			ch = cin.get();
			if(ch ==':' && mode == "NORMAL MODE") //switch to command mode
			{
				mode = "COMMAND MODE";
				xcurr = getWinRow()-winoffset+1;
				posCursor(xcurr,ycurr);
				display("printcurrentdir");
				display("---");
				// Switching ON the typing on terminal
				term.c_lflag |= ECHO;
				term.c_lflag |= ICANON;
				tcsetattr(fileno(stdin), 0, &term);

				char inChar;
				int flag = 0;
				while(1)
				{
					string commandToRun;
					
					while(1)
					{
						if(kbhit())
						{
							inChar = getchar();
							if(inChar == 27) //exit COMMAND MODE on escape and got to NORMAL MODE
							{
								mode = "NORMAL MODE";
								cout << "\033[2J\033[1;1H"; // to clear screen
								xcurr = 1;
								posCursor(xcurr,ycurr);

								// Switching OFF the typing on terminal for NORMAL MODE
								term.c_lflag &= ~ECHO;
								term.c_lflag &= ~ICANON;
								tcsetattr(fileno(stdin), 0, &term);

								fileCount = 0;
								listOfFiles.clear();
								c = 0;
								listAllFiles(currentPath.c_str());
								flag = 1;
								break;
							}
							else if(inChar == 127)
							{
								cout<<"\b \b";
								commandToRun  = commandToRun.substr(0,commandToRun.length() - 1);
							}
							else if(inChar != '\n'){
								commandToRun += inChar;
								cout<<inChar;
							}
							else if(inChar == '\n')
							{
								cout<<endl;
								break;
							}
							
						
						}
					}
					if(flag == 1)
					{
						break;
					}
					if(commandToRun == "quit") //quit program on quit command
					{
						cout << "\033[2J\033[1;1H";
						return 0;
					}
					
					string typeOfCommand = getCommandType(commandToRun); 
					
					//commands - 
					
					if(typeOfCommand == ""){}
					else if(typeOfCommand == "create_file")
					{
						string pathOfFile, newFileName;
						int spacecount = 0;
						for(int i = 0; i < commandToRun.length(); i++)
						{
							if(commandToRun[i] == ' ')
							 spacecount++;
							if(spacecount >= 2) break;
						}
						if(spacecount == 1)
							pathOfFile = currentPath;
						else
							pathOfFile = getFullPath(arg2(commandToRun));
						newFileName = arg1(commandToRun);
						if(validPath(pathOfFile))
							createNewFile(pathOfFile,newFileName);
						else
						{
							cout<<"Invalid destination path!\n";
							cout<<"Press any key to continue..";
							getch();
							clearCommandMode();
						} 
						clearCommandMode();
					}
					else if(typeOfCommand == "create_dir")
					{
						string pathOfDir, newDirName;
						int spacecount = 0;
						for(int i = 0; i < commandToRun.length(); i++)
						{
							if(commandToRun[i] == ' ')
							 spacecount++;
							if(spacecount >= 2) break;
						}
						if(spacecount == 1)
							pathOfDir = currentPath;
						else
							pathOfDir = getFullPath(arg2(commandToRun));
						newDirName = arg1(commandToRun);
						if(validPath(pathOfDir))
						{
							
							if(!findInCurrent(pathOfDir, pathOfDir+newDirName + "/", "dir"))
								createNewDir(pathOfDir, newDirName);
							else{
								cout<<"Directory already exists!\n";
								cout<<"Press any key to continue..";
								getch();
								clearCommandMode();
							} 
						}
							
						else
						{
							cout<<"Invalid destination path!\n";
							cout<<"Press any key to continue..";
							getch();
							clearCommandMode();
						} 
						clearCommandMode();
					}
					else if(typeOfCommand == "rename")
					{
						string oldName,newName;
						oldName = getFullPath(arg1(commandToRun));
						newName = getFullPath(arg2(commandToRun));
						
						if(oldName == currentPath)
						{
							cout<<"Can't rename current directory\n";
							cout<<"Press any key to continue..";
							getch();
							clearCommandMode();
						}
						else if(findInCurrent(currentPath,newName,"file"))
						{
							cout<<"File already exists\n";
							cout<<"Press any key to continue..";
							getch();
							clearCommandMode();
						}
						else if(rename(oldName.c_str(),newName.c_str()) != 0)
						{
							perror("Error in renaming");
							cout<<"Press any key to continue..";
							getch();
							clearCommandMode();
						}
						clearCommandMode();
					}
					else if(typeOfCommand == "delete_file")
					{
						string fileToDelete;
						fileToDelete = getFullPath(arg1(commandToRun));
						deleteFile(fileToDelete);
						cout<<"Press any key to continue..";
						getch();
						clearCommandMode();
					}
					else if(typeOfCommand == "delete_dir")
					{
						string fileToDelete;
						fileToDelete = getFullPath(arg1(commandToRun));
						if(fileToDelete == currentPath)
						{
							cout<<"Can't delete current directory\n";
						}
						else deletingFunction(fileToDelete);
						cout<<"Press any key to continue..";
						getch();
						clearCommandMode();
					}
					else if(typeOfCommand == "copy")
					{
						string srcfile, desdir;
						desdir = getFullPath(arg2(commandToRun));
						vector<string> srcs = getAllSrcs(commandToRun);
						
						if(validPath(desdir))
						{
							for(int i = 0; i < srcs.size(); i++)
							{
								struct stat filesInfo;
								stat(getFullPath(srcs[i]).c_str(),&filesInfo);	
								if(S_ISDIR(filesInfo.st_mode))
								{
									string fullPathSrc = getFullPath(srcs[i]);
									string srcParent = getParentDir(fullPathSrc);
									if(!validPath(fullPathSrc) || !findInCurrent(srcParent, fullPathSrc, "dir"))
									{
										cout<<"Source not found\n";
									}else if(!findInCurrent(desdir, fullPathSrc, "dir"))
									{	
										copyingFunction(fullPathSrc,desdir);
									}
									else cout<<"Directory already exists!\n";
								}
								else
								{
									string fullPathSrc = getFullPath(srcs[i]);
									if(findInCurrent(getParentOfFile(fullPathSrc),fullPathSrc,"file"))
										copyFile(fullPathSrc,desdir);
									else cout<<"Source not found\n";
								}
							}
						
						}else cout<<"Invalid destination path\n";
						cout<<"Press any key to continue..";
						getch();
						clearCommandMode();
					}
					else if(typeOfCommand == "move")
					{
						string srcfile,desdir;
						desdir = getFullPath(arg2(commandToRun));
						vector<string> srcs = getAllSrcs(commandToRun);
						if(validPath(desdir))
						{
							for(int i = 0; i < srcs.size(); i++)
							{
								struct stat filesInfo;
								stat(getFullPath(srcs[i]).c_str(),&filesInfo);	
								if(S_ISDIR(filesInfo.st_mode))
								{
									string fullPathSrc = getFullPath(srcs[i]);
									string srcParent = getParentDir(fullPathSrc);
									if(!validPath(fullPathSrc) || !findInCurrent(srcParent, fullPathSrc, "dir"))
									{
										cout<<"Source not found\n";
									}else if(fullPathSrc == currentPath){
										cout<<"Can't move current directory\n";
									}else if(!findInCurrent(desdir,fullPathSrc,"dir"))
									{
										copyingFunction(fullPathSrc,desdir);
										deletingFunction(fullPathSrc);
									}
									else cout<<"Directory already exists!\n";
									
								}
								else
								{
									string fullPathSrc = getFullPath(srcs[i]);
									if(findInCurrent(getParentOfFile(fullPathSrc),fullPathSrc,"file"))
									{
										copyFile(fullPathSrc,desdir);
										if(remove(fullPathSrc.c_str())!=0)
										{
											perror("Error in moving");
										}
									}
									else cout<<"Source not found\n";
									
								}
							}
						}
						else cout<<"Invalid destination path\n";
						cout<<"Press any key to continue..";
						getch();
						clearCommandMode();
					}
					else if(typeOfCommand == "search")
					{
						string toSearch;
						toSearch = arg1(commandToRun);
						if(searchInDir(currentPath,toSearch)) cout<<"True"<<endl;
						else cout<<"False"<<endl;
						cout<<"Press any key to continue..";
						getch();
						clearCommandMode();
					}
					else if(typeOfCommand == "goto")
					{
						string gotoPath = getFullPath(arg1(commandToRun));
						if(validPath(gotoPath))
						{
							listOfFiles.clear();
							c = 0;
							navStack.push(gotoPath);
							currentPath = gotoPath;
							listAllFiles(currentPath);
							while(!navStackForward.empty()) navStackForward.pop();
							xcurr= getWinRow() - winoffset + 4; //+4 because we want to print below mode and directory
							posCursor(xcurr,ycurr);
						}else
						{
							cout<<"Invalid Path\n";
							cout<<"Press any key to continue..";
							getch();
							clearCommandMode();
						} 
						
					}
					else
					{
						cout<<"\033[1;31mInvalid Command\033[0m"<<endl;
						cout<<"Press any key to continue..";
						getch();
						clearCommandMode();
					}
				}
				
				
			}
			else if(ch == ':' && mode == "COMMAND MODE") //switch to command mode
			{
				mode="NORMAL MODE";
				cout << "\033[2J\033[1;1H";
				xcurr = 1;
				posCursor(xcurr,ycurr);
				term.c_lflag &= ~ECHO;
				term.c_lflag &= ~ICANON;
				tcsetattr(fileno(stdin), 0, &term);
				fileCount = 0;
				listOfFiles.clear();
				c = 0;
				listAllFiles(currentPath.c_str());
			}
			else if(ch == 'A' && mode == "NORMAL MODE") //handle scrolling up
			{	
				if(c1 > 0 && xcurr >= 2)
				{
					xcurr--;
					if(xcurr <= 1) xcurr = 1;
					posCursor(xcurr,ycurr);
				}
				else if(xcurr == 1 && c > 0)
				{
					c--;
					
					if(c1 > 0)
						c1--;
					xcurr = 1;
					posCursor(xcurr,ycurr);
					cout << "\033[2J\033[1;1H";
					for(int i=c;i<=c1;i++)
					{
						display(listOfFiles[i]);
						
					}
					posCursor(getWinRow() - winoffset + 1,ycurr);
					display("printcurrentdir");
					display("---");
					xcurr = 1;
					posCursor(xcurr,ycurr);
				}else
				{
					xcurr=1;
					posCursor(xcurr,ycurr);
				}
			}
			
			else if(ch == 'B' && mode == "NORMAL MODE") //handle scrolling down
			{
				if(xcurr < getWinRow() - winoffset && xcurr <= c1)
				{
					xcurr++;
					posCursor(xcurr,ycurr);
				}
				else if(c1 < fileCount-1)
				{	
					c1++;
					c++;
					xcurr = 1;
					posCursor(xcurr,ycurr);
					cout << "\033[2J\033[1;1H";
					for(int i = c; i <= c1; i++)
					{
						display(listOfFiles[i]);
							
					}
					posCursor(getWinRow() - winoffset + 1, ycurr);
					display("printcurrentdir");
					display("---");
					xcurr = getWinRow() - winoffset;
					posCursor(xcurr,ycurr);				
				}
				else
				{
					c1 = fileCount - 1;
					xcurr = fileCount < getWinRow() - winoffset ? fileCount: getWinRow() - winoffset;
					posCursor(getWinRow() - winoffset + 1, ycurr);
					display("printcurrentdir");
					display("---");
					posCursor(xcurr,ycurr);
				}
			}
			else if(ch == 'q') //quit program on q press
			{
				term.c_lflag |= ECHO;
				term.c_lflag |= ICANON;
				tcsetattr(fileno(stdin), 0, &term);
				cout << "\033[2J\033[1;1H";
				break;
			}
			else if(ch=='\n' && mode=="NORMAL MODE") //enter file or folder
			{
				struct stat filesInfo;
				int indextoenter = xcurr+c-1;
				if(stat((listOfFiles[indextoenter]).c_str(),&filesInfo)==0)
				{	
					if((filesInfo.st_mode & S_IFMT) == S_IFREG)
					{
						pid_t processID = fork();
				
						if(processID == 0)
						{
							execlp("xdg-open","xdg-open",listOfFiles[indextoenter].c_str(),NULL);
						}
					}else
					{
						if(indextoenter < fileCount) //indextoenter==0 to prevent going forward from mode line
						{
							string newPath;
							if(getFolderName(listOfFiles[indextoenter]+"/") == "..")
							{
								string pDir = getParentDir(currentPath);
								newPath = pDir;
								navStack.push(pDir);
								currentPath = pDir;
							}
							else if(getFolderName(listOfFiles[indextoenter] + "/") == ".")
							{
								newPath = currentPath;
							}
							else
							{
								newPath = listOfFiles[indextoenter]+"/";
								navStack.push(newPath);
								currentPath=newPath;
							}
							while(!navStackForward.empty()) navStackForward.pop();
							listOfFiles.clear();
							c=0;
							listAllFiles(newPath.c_str());
						}
					}
				}
				
				
			}
			else if(ch=='D' && mode=="NORMAL MODE") //handle going back
			{
				if(!navStack.empty())
					{
						
						navStack.pop();
						string prevPath=currentPath;
						if(!navStack.empty())
						{
							prevPath = navStack.top();
							navStackForward.push(currentPath);
							currentPath=prevPath;
							
						}else{
							navStack.push(currentPath);
						}
						listOfFiles.clear();
						c=0;
						listAllFiles(prevPath.c_str());
					}else
					{
						listOfFiles.clear();
						c=0;
						listAllFiles(currentPath.c_str());
					}
				
				
			}
			else if(ch=='C' && mode=="NORMAL MODE")//handle going forward
			{
				
				if(!navStackForward.empty())
				{
					string nextPath = navStackForward.top();
					navStackForward.pop();
					navStack.push(nextPath);
					currentPath = nextPath;
					listOfFiles.clear();
					c=0;
					listAllFiles(nextPath.c_str());
				}else
					{
						listOfFiles.clear();
						c=0;
						listAllFiles(currentPath.c_str());
					}
			}
			else if(ch==127 && mode=="NORMAL MODE") //handle going to parent directory (one level up)
			{
				string parentDir = getParentDir(currentPath);
				if(navStack.top()!="/home/")
					navStack.push(parentDir);
				currentPath=parentDir;
				while(!navStackForward.empty()) navStackForward.pop();
				listOfFiles.clear();
				c=0;
				listAllFiles(parentDir.c_str());
				
			}
			else if(ch=='h' && mode=="NORMAL MODE") //handle going home
			{
				uid_t uid = geteuid();
				string useridpc = getpwuid(uid)->pw_name;
	  			string gohomedir = "/home/"+ useridpc +"/";
	  			while(!navStackForward.empty()) navStackForward.pop();
	  			if(navStack.top()!=gohomedir)
	  				navStack.push(gohomedir);
				currentPath=gohomedir;
				listOfFiles.clear();
				c=0;
				listAllFiles(gohomedir.c_str());
			}
		
		}
		
	}
	
	

	return 0;
}



