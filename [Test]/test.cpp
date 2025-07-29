#include<Config.hpp>
#include<GlobalConfig.hpp>

using namespace std;
using namespace Convention;

int main()
{
	cout << ProjectConfig().LoadProperties().FindItem<std::string>("test", "test") << endl;
}
