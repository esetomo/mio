#include <mio/definitions.h>
#include <mio/utility/string.h>

std::string strip(const std::string &s) 
{
	int left = 0;
	while (left < (int)s.length() && isspace(s[left]))
		left++;
	int right = (int)s.length()-1;
	while (right >= 0 && isspace(s[right]))
		right--;
	std::string result = "";
	FOR2(k, left, right)
		result += s[k];
	return result;
}

std::vector<std::string> split(const std::string &s, char delim) 
{
	std::vector<std::string> result;
	std::vector<int> delim_positions;
	std::string ss = strip(s);
	
	std::string candidate = "";
	FOR(i, (int)ss.length())
	{
		if (ss[i] == delim)
		{
			result.push_back(candidate);
			candidate = "";
		}
		else
			candidate += ss[i];
	}
	result.push_back(candidate);

	return result;
}