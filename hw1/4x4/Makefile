all:
#	g++ -O3 -std=c++11 -Wno-write-strings gen_db.cpp -o bin/gen_db
	g++ -O3 -std=c++11 -Wno-write-strings new_db.cpp -o bin/new_db
	g++ -O3 -std=c++11 -Wno-write-strings new_db_sub.cpp -o bin/new_db_sub
	g++ -O3 -Wno-write-strings -std=c++11 new_run.cpp -o bin/15_puzzle
	g++ -O3 -Wno-write-strings -std=c++11 new_run_sub.cpp -o bin/15_puzzle_sub
	g++ -O3 -Wno-write-strings -std=c++11 new_run_sub_fh.cpp -o bin/15_puzzle_sub_fh
	g++ -O3 -Wno-write-strings -std=c++11 new_run_sub_sh.cpp -o bin/15_puzzle_sub_sh
	
clean:
	rm bin/*
	rm db/*
	rm log/*
	