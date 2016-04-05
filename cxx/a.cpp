#include <map>
#include <cstdio>
#include <vector>
#include <memory>

using namespace std;

int main()
{
	unique_ptr<int> a = unique_ptr<int>(new int(4));
	
	unique_ptr<int> b = unique_ptr<int>(new int(*a.get()));
	printf("%d\n", *b.get());
}
