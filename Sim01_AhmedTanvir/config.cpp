#ifndef CONFIG_CPP_
#define CONFIG_CPP_
#include "config.h"


configFile c; //creating c object of configFile class
metaFile m; //creating m object of metaFile class
ofstream fout; //opening the output file
bool signal_f, signal_m; //signal_f to write output in the file, signal_m to show output in the monitor

void run(string f){
	string s;
	ifstream fin;
	fin.open(f.c_str()); //confg file is opened

	while(fin){
    getline(fin, s); //reading from file, reads every line, terminates when finds newline character
        
    for(int i = 0; i < s.length(); i++){ //checking every character
    	if (s[i] == ':'){ //when finds a colon, the substring from starting to before colon will be matched to and the values will be stored in the object
    		if (small(s.substr(0, i)) == "version/phase"){
    			c.ver = s.substr(i+2, s.length()-2); //stored as string
    			//cout << c.ver << endl;
    		}
    		if (small(s.substr(0, i)) == "file path"){
    			c.file_path = s.substr(i+2, s.length()-2); ////stored as string
    			//cout << c.file_path << endl;
    		}
    		if (small(s.substr(0, i)) == "processor cycle time (msec)"){
    			c.proc = string2int(s.substr(i+2, s.length()-2)); //converted to int, see string to int function below
    			//cout << c.proc << endl;
    		}
    		if (small(s.substr(0, i)) == "monitor display time (msec)"){
    			c.monitor = string2int(s.substr(i+2, s.length()-2)); //converted to int, see string to int function below
    			//cout << c.monitor << endl;
    		}
    		if (small(s.substr(0, i)) == "hard drive cycle time (msec)"){
    			c.hdd = string2int(s.substr(i+2, s.length()-2)); //converted to int, see string to int function below
    			//cout << c.hdd << endl;
    		}
    		if (small(s.substr(0, i)) == "printer cycle time (msec)"){
    			c.printer = string2int(s.substr(i+2, s.length()-2)); //converted to int, see string to int function below
    			//cout << c.printer << endl;
    		}
    		if (small(s.substr(0, i)) == "keyboard cycle time (msec)"){
    			c.keyboard = string2int(s.substr(i+2, s.length()-2)); //converted to int, see string to int function below
    			//cout << c.keyboard << endl;
    		}
    		if (small(s.substr(0, i)) == "memory cycle time (msec)"){
    			c.mem = string2int(s.substr(i+2, s.length()-2)); //converted to int, see string to int function below
    			//cout << c.mem << endl;
    		}
    		if (small(s.substr(0, i)) == "system memory (kbytes)"){
    			c.sys_mem = string2int(s.substr(i+2, s.length()-2)); //converted to int, see string to int function below
    			//cout << c.sys_mem << endl;
    		}
    		if (small(s.substr(0, i)) == "log"){
    			c.log = s.substr(i+2, s.length()-2); ////stored as string
    			//cout << c.log << endl;
    		}
    		if (small(s.substr(0, i)) == "log file path"){
    			c.log_path = s.substr(i+2, s.length()-2); ////stored as string
    			//cout << c.log_path << endl;
    		}
    	}
    }
  }

  fin.close();
  output();

  string md; //will be storing the metadata file
  ifstream mdf; 
  mdf.open(c.file_path.c_str()); //metadata file is opened

  while(mdf){
  	getline(mdf, md); //md is each line from the text
  	//cout << md << endl;
  	int flag = 0; //initial ptr of the string we want

  	for (int i = 0; i < md.length(); i++){
  		if (md[i] == ')'){
  			string tmp = md.substr(flag, i+1-flag);
  			if(tmp[0] == ' '){
  				tmp = tmp.substr(1,tmp.length()-1);
  			}
  			flag = i+4;
  			//cout << tmp << endl;
  			int val_md;
        //checking to see if there is any semicolon, colon, full-stop, comma, or space
  			if(md[i+2] == ';' or md[i+2] == ',' or md[i+2] == '.' or md[i+2] == ':' or md[i+2] == ' '){
  				val_md = string2int(md.substr(i+1,1));
  				//cout << val_md << endl;
  			}
        //works for single and double digit integers 
  			if (md[i+3] == ';' or md[i+3] == ',' or md[i+3] == '.' or md[i+3] == ':' or md[i+3] == ' '){
  				val_md = string2int(md.substr(i+1,2));
  			}
  			
  			if (small(tmp) == "s(start)"){
  				m.Ss = val_md;
  				//cout << m.Ss << endl;  				
  			}
  			if (small(tmp) == "s(end)"){
  				m.Se = val_md;
  				//cout << m.Se << endl;  				
  			}
  			if (small(tmp) == "a(start)"){
  				m.As = val_md;
  				//cout << m.As << endl;  				
  			}
  			if (small(tmp) == "A(end)"){
  				m.Ae = val_md;
  				//cout << m.Ae << endl;  				
  			}
  			if (small(tmp) == "p(run)"){
  				m.P = val_md;
  				if (signal_f and !signal_m){
  					fout << "P(run)" << m.P << " - " << m.P * c.proc << " ms" << endl;
  				}
  				if(signal_m and !signal_f){
  					cout << "P(run)" << m.P << " - " << m.P * c.proc << " ms" << endl;
  				}
  				if(signal_f and signal_m){
  					fout << "P(run)" << m.P << " - " << m.P * c.proc << " ms" << endl;
  					cout << "P(run)" << m.P << " - " << m.P * c.proc << " ms" << endl;
  				}
  				//cout << m.P << endl;  				
  			}
  			if (small(tmp) == "i(hard drive)"){
  				m.Id = val_md;
  				if(signal_f and !signal_m){
  					fout << "I(hard drive)" << m.Id << " - " << m.Id * c.hdd << " ms" << endl;
  				}
  				if(signal_m and !signal_f){
  					cout << "I(hard drive)" << m.Id << " - " << m.Id * c.hdd << " ms" << endl;
  				}
  				if(signal_f and signal_m){
  					fout << "I(hard drive)" << m.Id << " - " << m.Id * c.hdd << " ms" << endl;
  					cout << "I(hard drive)" << m.Id << " - " << m.Id * c.hdd << " ms" << endl;
  					//cout << m.Id << endl;  				
  				}
  				
  			}
  			if (small(tmp) == "i(keyboard)"){
  				m.Ik = val_md;
  				if(signal_f and !signal_m){
  					fout << "I(keyboard)" << m.Ik << " - " << m.Ik * c.keyboard << " ms" << endl;
  				}
  				if (signal_m and !signal_f){
  					cout << "I(keyboard)" << m.Ik << " - " << m.Ik * c.keyboard << " ms" << endl;	
  				}
  				if(signal_f and signal_m){
  					fout << "I(keyboard)" << m.Ik << " - " << m.Ik * c.keyboard << " ms" << endl;
  					cout << "I(keyboard)" << m.Ik << " - " << m.Ik * c.keyboard << " ms" << endl;
  					//cout << m.Ik << endl;  					
  				}
  				
  			}
  			if (small(tmp) == "o(hard drive)"){
  				m.Od = val_md;
  				if(signal_f and !signal_m){
  					fout << "O(hard drive)" << m.Od << " - " << m.Od * c.hdd << " ms" << endl;	
  				}
  				if (signal_m and !signal_f){
  					cout << "O(hard drive)" << m.Od << " - " << m.Od * c.hdd << " ms" << endl;	
  				}
  				if(signal_f and signal_m){
  					fout << "O(hard drive)" << m.Od << " - " << m.Od * c.hdd << " ms" << endl;
  					cout << "O(hard drive)" << m.Od << " - " << m.Od * c.hdd << " ms" << endl;
  				}
  			}
  			if (small(tmp) == "o(monitor)"){
  				m.Om = val_md;
  				if (signal_f and !signal_m){
  					fout << "O(monitor)" << m.Om << " - " << m.Om * c.monitor << " ms" << endl;
  				}
  				if(signal_m and !signal_f){
  					cout << "O(monitor)" << m.Om << " - " << m.Om * c.monitor << " ms" << endl;
  				}	
  				if(signal_f and signal_m){
  					fout << "O(monitor)" << m.Om << " - " << m.Om * c.monitor << " ms" << endl;
  					cout << "O(monitor)" << m.Om << " - " << m.Om * c.monitor << " ms" << endl;
  					//cout << m.Om << endl;  				
  				}
  				
  			}
  			if (small(tmp) == "o(printer)"){
  				m.Op = val_md;
  				if (signal_f and !signal_m){
  					fout << "O(printer)" << m.Op << " - " << m.Op * c.printer << " ms" << endl;
  				}
  				if (signal_m and !signal_f){
  					cout << "O(printer)" << m.Op << " - " << m.Op * c.printer << " ms" << endl;
  				}
  				if (signal_f and signal_m){
  					fout << "O(printer)" << m.Op << " - " << m.Op * c.printer << " ms" << endl;
  					cout << "O(printer)" << m.Op << " - " << m.Op * c.printer << " ms" << endl;
  					//cout << m.Om << endl;  				
  				}
  				
  			}
  			if (small(tmp) == "m(allocate)"){
  				m.Ma = val_md;
  				if (signal_f and !signal_m){
  					fout << "M(allocate)" << m.Ma << " - " << m.Ma * c.mem << " ms" << endl;	
  				}
  				if (signal_m and !signal_f){
  					cout << "M(allocate)" << m.Ma << " - " << m.Ma * c.mem << " ms" << endl;	
  				}
  				if (signal_f and signal_m){
  					fout << "M(allocate)" << m.Ma << " - " << m.Ma * c.mem << " ms" << endl;
  					cout << "M(allocate)" << m.Ma << " - " << m.Ma * c.mem << " ms" << endl;
  					//cout << m.Ma << endl;  				
  				}
  				
  			}
  			if (small(tmp) == "m(cache)"){
  				m.Mc = val_md;
  				if (signal_f and !signal_m){
  					fout << "M(cache)" << m.Mc << " - " << m.Mc * c.mem << " ms" << endl;
  				}
  				if (signal_m and !signal_f){
  					cout << "M(cache)" << m.Mc << " - " << m.Mc * c.mem << " ms" << endl;
  				}
  				if (signal_f and signal_m){
  					fout << "M(cache)" << m.Mc << " - " << m.Mc * c.mem << " ms" << endl;
  					cout << "M(cache)" << m.Mc << " - " << m.Mc * c.mem << " ms" << endl;
  					//cout << m.Mc << endl;  				
  				}
  				
  			}
  		}
  	}

  }
  mdf.close(); //closing metadata file after reading 
  fout.close(); //closing output file after reading
}


void output(){
	if(small(c.log) == "log to both"){
		signal_f = true;
		signal_m = true;
		write_file();
		write_monitor();
	}
	if(small(c.log) == "log to file"){
		signal_f = true;
		signal_m = false;
		write_file();
		//fout.close();
	}
	if(small(c.log) == "log to monitor"){
		signal_m = true;
		signal_f = false;
		write_monitor();
	}
}

void write_file (){
	
	//ofstream fout;
	fout.open(c.log_path.c_str());
	fout << "Configuration File Data" << endl;
	fout << "Processor = " << c.proc << " ms/cycle" << endl;
	fout << "Monitor = " << c.monitor << " ms/cycle" << endl;
	fout << "Hard Drive = " << c.hdd << " ms/cycle" << endl;
	fout << "Printer = " << c.printer << " ms/cycle" << endl;
	fout << "Keyboard = " << c.keyboard << " ms/cycle" << endl;
	fout << "Memory = " << c.mem << " ms/cycle" << endl << endl;
	//fout << "System Memory = " << c.sys_mem << " kbytes" << endl;
	fout << "Meta-Data Metrics" << endl;
	//fout.close();
}

void write_monitor(){
	cout << "Configuration File Data" << endl;
	cout << "Processor = " << c.proc << " ms/cycle" << endl;
	cout << "Monitor = " << c.monitor << " ms/cycle" << endl;
	cout << "Hard Drive = " << c.hdd << " ms/cycle" << endl;
	cout << "Printer = " << c.printer << " ms/cycle" << endl;
	cout << "Keyboard = " << c.keyboard << " ms/cycle" << endl;
	cout << "Memory = " << c.mem << " ms/cycle" << endl << endl;
	//fout << "System Memory = " << c.sys_mem << " kbytes" << endl;
	cout << "Meta-Data Metrics" << endl;
}

//taking a string and making it in small-letter format
string small(string s){
	for (int i = 0; i < s.length(); i++){
		if(s[i] >= 'A' and s[i] <= 'Z'){
			s[i] = tolower(s[i]);
		}
	}
	return s;
}
	

//taking a integer in string-format and turning it into an interger-format
int string2int(string s){
	int i;
	stringstream convert(s);
	convert >> i;
	return i;
}

#endif