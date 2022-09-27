#include "utils.h"

long GetFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}


void sendFile(int newsocket, std::string filename)
{
	std::string line;
  	std::ifstream myfile(filename, std::ios::binary);
	char buffer[1024];
	long fileSize = GetFileSize(filename);
	std::string ssize = std::to_string(fileSize);

	send(newsocket, ssize.c_str(), ssize.length(), 0);

	if (myfile.is_open())
	{
		while (myfile)
		{
			myfile.read(buffer, 1024);
			send(newsocket, buffer, 1024, 0);
			std::cout << "gcount" << myfile.gcount() << '\n';
		}
		myfile.close();
	}
	else std::cout << "Unable to open input file"; 
}

void getFile(int newsocket, long fileSize, std::string filename){
    // get in the entire file
    char buffer[1024] = { 0 };
    int valread;
  	std::fstream outfile;
	outfile.open (filename, std::ios::binary|std::ios::out);
	if (outfile.is_open())
	{
		while(fileSize > 0)
		{
			long readSize = fileSize > 1024 ? 1024 : fileSize;
			size_t before = outfile.tellp();
			valread = read(newsocket, buffer, readSize);
			outfile.write(buffer, readSize);
			size_t after = outfile.tellp();
			size_t numBytesWritten = after - before;
			fileSize -= numBytesWritten;
			std::cout << "wrote : " << numBytesWritten << "\n";
		}
		outfile.close();
	}
	else std::cout << "Unable to open output file";
}