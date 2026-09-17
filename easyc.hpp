
#include <curl/curl.h>
#include "iostream"
#include "string"
#include "fstream"
#include <filesystem>
using namespace std;
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//GetString function is function for geting information from link or getting htm info for parsing.
string GetString(string url){
    CURL *handle;
    
    CURLcode res;
    string readBuffer;
    handle = curl_easy_init();
    if(handle) {
        //set url
        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
        //set writing/callbacl fuction
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &readBuffer);
        //perform writing
        res = curl_easy_perform(handle);
        //clean up
        curl_easy_cleanup(handle);
        //return string
        return readBuffer;
  }else{
    return "curl init error";
  }
}
//Download function is Downloading file by URL, url, FileName like (img.png) Path such as /home/user/folder/
string Download(const string& url,
              const string& fileName,
              const string& path = "None")
{
    
    string data = GetString(url);
   

    filesystem::path fullPath;
    //if there no path we downloading file in sae folder where is app.
    if (path != "None") {
        //if there is a path but it dont have / in end we add it.
        if (*(path.end() - 1) != '/'){
            string path = path + '/';
        }
        //path to create a file
        fullPath = filesystem::path(path) / fileName;

        // Create the directory if it does not exist
        filesystem::create_directories(fullPath.parent_path());
    } else {
        fullPath = fileName;
    }
    //opening file to write
    ofstream myFile(fullPath);
    //check permisiond and path for mistakes
    if (!myFile) {
        cerr << "Failed to create or open: "
                  << fullPath << '\n';
        return "Failed to create";
    }
    //writing data
    myFile << data;
    //check if writing is posibble
    if (!myFile) {
        cerr << "Failed to write to: "
                  << fullPath << '\n';
        return "Failed to write";
    }
    //return message
    return "File saved";
}
