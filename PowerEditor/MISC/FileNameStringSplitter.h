// This file is part of Notepad++ project
// Copyright (C)2021 Don HO <don.h@free.fr>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

typedef Vector<String> stringVector;

class FileNameStringSplitter
{
public:
	FileNameStringSplitter(const char *fileNameStr)
	{
		char *pStr = Null;
		bool isInsideQuotes = false;
		const int filePathLength = MAX_PATH;

		char str[filePathLength];
		int i = 0;
        bool fini = false;

		for (pStr = (char *)fileNameStr ; !fini ; )
		{
			if (i >= filePathLength)
				break;

			switch (*pStr)
			{
				case '"':
				{
					if (isInsideQuotes)
					{
						str[i] = '\0';
                        if (str[0])
							_fileNames.push_back(String(str));
						i = 0;
					}
					isInsideQuotes = !isInsideQuotes;
					pStr++;
					break;
				}

				case ' ':
				{
					if (isInsideQuotes)
					{
						str[i] = *pStr;
						i++;
					}
					else
					{
						str[i] = '\0';
                        if (str[0])
							_fileNames.push_back(String(str));
						i = 0;
					}
                    pStr++;
					break;
				}

                case '\0':
				{
                    str[i] = *pStr;
                    if (str[0])
						_fileNames.push_back(String(str));
                    fini = true;
					break;
				}

				default :
				{
					str[i] = *pStr;
					i++; pStr++;
					break;
				}
			}
		}
	}

	const stringVector& getFileNames() const {
		return _fileNames;
	};

	const char * getFileName(size_t index) const {
		if (index >= _fileNames.size())
			return Null;
		return _fileNames[index].Begin();
	}

	int size() const {
		return int(_fileNames.size());
	}

private :
	stringVector _fileNames;
};
