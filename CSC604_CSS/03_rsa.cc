#include<iostream>
#include<cmath>
#include<cstring>
#define SIZE 100
#define lld  long long int
using namespace std;

 lld getCoPrime( lld fn){
	for(lld i = 2; i<fn; i++){
		if(fn%i!=0){
			return i;
		}
	}
	return fn+1;
}
 lld getInverse( lld e,  lld fn){
	e = e % fn;
	for(int i = 1; i<fn; i++){
		if((e*i)%fn == 1)
			return i;
	}
	return 1;
}

int main(){
	 lld p, q, n, fn, e, d;
	cout<<"Enter p and q:\n";
	cin>>p>>q;
	n = p*q;
	fn = (p-1)*(q-1);
	e = getCoPrime(fn);
	d = getInverse(e, fn);
	cout<<"fn: "<<fn<<"\tn: "<<n<<"\te: "<<e<<"\td: "<<d<<endl;

	// Read plain text
	char input[SIZE];
	cout<<"Enter the plain text: ";
	cin>>input;
	lld len = strlen(input);
	lld output[len], dinput[len];
	for(lld i = 0; i<len; i++){
		output[i] = (lld)pow((lld)input[i], e)%n;
	}

	// Encrypted text
	for(lld i = 0; i<len; i++){
		dinput[i] = (lld)pow((lld)output[i], d)%n;
	}
	cout<<"Decrypted text: "<<dinput<<endl;

	// Display all values
	cout<<"Input\tEncrypted\tDecrypted\n";
	for(lld i=0; i<len; i++){
		cout<<(lld)input[i]<<"\t"<<output[i]<<"\t\t"<<dinput[i]<<endl;
	}
	return 0;
}