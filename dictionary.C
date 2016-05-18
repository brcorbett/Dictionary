//Dictionary.C
//Brock.Corbett

#include<iostream>
#include<string>
#include<vector>
#include<fstream>


#include "dictionary.h"

bool Dictionary::failure = false;
bool Dictionary::success = true;

//constructor

Dictionary::Dictionary():maxWordsInDict(10000), maxWordsPerFile(100)
{
    totalWordsInDict = 0;

    for (int i = 0; i < 25; i++)
      {
	numOfWordsInFile[i] = 0;
      }
}

//------------------------------------------------------------------

//constructor with inputs

Dictionary::Dictionary(int dictMaxWords, int fileMaxWords):maxWordsInDict(dictMaxWords), maxWordsPerFile(fileMaxWords)
{
    totalWordsInDict = 0;

    for (int i = 0; i < 25; i++)
      {
        numOfWordsInFile[i] = 0;
      }
}

//------------------------------------------------------------------

//After a string is sent to this function, it checks if it's already in the dictionary, if it's not, it is added.

bool Dictionary::AddAWord(string word)
{
  bool status;

  status = SearchForWord(word);

  if (status == false)
    {
      string fileName = "#.txt"; 
      fileName[0] = toupper(word[0]);      
      ofstream fout;
      fout.open(fileName.data(), ios::app); //opens "letter".Txt
      if (totalWordsInDict < maxWordsInDict)
	{
	  word = lowerchar(word);
	  fout << word << " " << endl;
	  totalWordsInDict++;
	  numOfWordsInFile[fileName[0] - 'A']++;
	  //cout << word << " successfully added" << endl;
	  fout.close();
	  return (Dictionary::success);
         } 
      else
	{
	  cout << "Too many words in specific file" << endl;
	  fout.close();
	  return (Dictionary::failure);
	}
    }
  else
    {
      //cout << "Could not add word: " << word << endl;
      return(Dictionary::failure);
    }
}
//--------------------------------------------------------------------

//opens up respective text file for letter, puts all the words in a vector and goes through each one until it finds the given word, then deletes the word, then readds the modified vector to the text file

bool Dictionary::DeleteAWord(string word)
{
  bool status;
  string fileName = "#.txt";
  fileName[0] = toupper(word[0]);

  ifstream fin;
  ofstream fout;

  vector<string>WordsFromFileVec;

  fin.open(fileName.data());

  word = lowerchar(word);

  status = SearchForWord(word);
  
  if (status == true)
    {
      string input;

      while (fin >> input)
	{
	  WordsFromFileVec.push_back(input);
	}
      for (int i = 0; i < WordsFromFileVec.size(); i++)
	{
	  if (word == WordsFromFileVec[i])
	    {
	      WordsFromFileVec.erase(WordsFromFileVec.begin()+i);
	      totalWordsInDict--;
	      numOfWordsInFile[fileName[0] - 'A']--;
      	    }
	}

      fout.open(fileName.data());

      for (int i = 0; i < WordsFromFileVec.size(); i++)
	{
	  fout << WordsFromFileVec[i] << " ";
	}

      fout.close();
      //cout << "Deleting " << word << " was a success!" << endl;
      return(Dictionary::success);
    }
  else
    {
      //cout << "Finding " << word << " was a failure" << endl;
      return (Dictionary::failure);
    }
}

//-------------------------------------------------------------

//opens up text file for letter and searches for given word, returns success if found

bool Dictionary::SearchForWord(string word)
{
  word = lowerchar(word);
  string fileName = "#.txt";
  fileName[0] = toupper(word[0]);

  ifstream fin;
  string input;

  //word = lowerchar(word);

  fin.open(fileName.data());
  
  while (fin >> input)
    {
      if (word == input)
	{
	  //fin.close();
	  //cout << "Success finding " << word  << endl;
	  return (Dictionary::success);
	}
      // fin >> input;
    }
  //cout << "Failure finding " << word << endl;
  return (Dictionary::failure);
}

//-------------------------------------------------------------

//opens up specific dictionary and prints out words from it

bool Dictionary::PrintAFileInDict(string fileName)
{
  ofstream fout;
  ifstream fin;
  string input, combined;

  fin.open(fileName.data());

  if (!fin)
    {
      cout << "Error: File" << fileName << " couldn't be opened" << endl;
      return (Dictionary::failure);
    }
  else
    {
      while(fin)
	{
	  combined = "";
     	  for (int i = 0; i < 5 && fin; i++)
	    {
	      fin >> input;
	      if(fin.eof()) break;
	      combined += (" " + input);
       	    }
	  cout << combined << endl;
      	}
      return (Dictionary::success);
    }
}

  
//----------------------------------------------------------------

//lowers the characters of the word, used when inputting into dictionary
 string Dictionary::lowerchar(string word)
  {
    for (int i = 0; i < word.size(); i++)
      {
	word[i] = tolower(word[i]); //changes the word into lowercase
      }
    return word;
  }
  
  //-------------------------------------------------------------------

//opens up file name, if word is not found in the dictionary, it is listed
  
bool Dictionary::SpellChecking(string fileName)
{
  ifstream fin;
  string input;
  bool status;

  fin.open(fileName.data());

  if (!fin)
    {
      cout << fileName << " could not be opened." << endl;
      return (Dictionary::failure); 
    }
  cout << "Words Not Found In The Dictionary: " << endl;
  cout << "-----------------------------------" << endl;
  while (fin >> input)
    {
      status = SearchForWord(input);

      if (status == false)
	{
	  cout << input << endl;
	}
      // fin >> input;
    }
  cout << "-----------------------------------" << endl;
  fin.close();
  return (Dictionary::success);	  
}

//----------------------------------------------------------------------
//opens up file of letter, searches to see if word is in it already, if it isn't, the word is added

bool Dictionary::InsertWordsIntoDict(string fileName)
{
  ifstream fin;
  ofstream fout;
  string input;
  bool status;
  
  fin.open(fileName.data());

  if (!fin)
    {
      cout << fileName << " cannot be opened." << endl;
      return (Dictionary::failure);
    }

  while (fin >> input)
    {
      status = SearchForWord(input);

      if (status == false)
	{
	  AddAWord(input);
	}
      //fin >> input;
    }
  fin.close();
  return (Dictionary::success);
}

//-----------------------------------------------------------------
//opens transaction file containing commands and queries

void Dictionary::ProcessTransactionFile()
{
  string fileName, command, query;
  ifstream fin;

  cout << "Enter a transaction file:" << endl;
  cin >> fileName;

  fin.open(fileName.data());

  if (!fin)
    {
      cout << "The file " << fileName << " cannot be opened" << endl;
    }

  fin >> command >> query;

  while (fin)
    {
      if (command == "AddW")
	{
	  AddAWord(query);
	}
      if (command == "DeleteW")
	{
	  DeleteAWord(query);
	}
      if (command == "SearchW")
	{
	  SearchForWord(query);
	}
      if (command == "PrintF")
	{
	  PrintAFileInDict(query);
	}
      if (command == "SpellCheck")
	{
	  SpellChecking(query);
	}
      if (command == "InsertF")
	{
	  InsertWordsIntoDict(query);
	}
      fin >> command >> query;
    }
  fin.close();
}

   

  
  
  
