#include <iostream>
#include <fstream>

class Car {
public:
	Car() = default;
	Car(const char* name, size_t name_size, size_t year_creation, size_t max_speed, size_t* adventures_km, size_t size) {
		if (!name) {
			throw std::exception("Invalid name");
		}

		nameSize = name_size;
		this->name = new char[nameSize];
		strcpy(this->name, name);
		yearCreation = year_creation;
		maxSpeed = max_speed;
		adventuresKM = new size_t[size];
		numberOfAdventures = size;
		for (int i = 0; i < size; i++) {
			adventuresKM[i] = adventures_km[i];
		}
	}

	~Car() {
		delete[] name;
		delete[] adventuresKM;
	}

	void writeToFile(std::ostream& os) {
		os.write((const char*)&nameSize, sizeof(nameSize));
		os.write(name, nameSize);
		os.write((const char*)&yearCreation, sizeof(yearCreation));
		os.write((const char*)&maxSpeed, sizeof(maxSpeed));
		os.write((const char*)&numberOfAdventures, sizeof(numberOfAdventures));
		os.write((const char*)adventuresKM, numberOfAdventures * sizeof(size_t));
	}

	void writeToFile(const char* filename) {
		std::ofstream ofs(filename, std::ios::binary);

		if (!ofs.is_open()) {
			throw std::invalid_argument("File does not exist");
		}

		writeToFile(ofs);


		ofs.close();
	}

	void readFromFile(std::istream& is) {
		is.read((char*)&nameSize, sizeof(nameSize));
		name = new char[nameSize];
		is.read(name, nameSize);
		is.read((char*)&yearCreation, sizeof(yearCreation));
		is.read((char*)&maxSpeed, sizeof(maxSpeed));
		is.read((char*)&numberOfAdventures, sizeof(numberOfAdventures));
		adventuresKM = new size_t[numberOfAdventures];
		is.read((char*)adventuresKM, sizeof(size_t) * numberOfAdventures);
	}

	void readFromFile(const char* filename) {
		std::ifstream ifs(filename, std::ios::binary);

		if (!ifs.is_open()) {
			throw std::invalid_argument("SUCKS");
		}

		readFromFile(ifs);

		ifs.close();
	}

private:
	char* name = 0;
	size_t nameSize = 0;
	size_t yearCreation = 0;
	size_t maxSpeed = 0;
	size_t* adventuresKM = nullptr;
	size_t numberOfAdventures = 0;
};



class BankAccount {
public:
	BankAccount() = default;
	BankAccount(const char* name_, const char* iban_, double balance_) :balance(balance_) {
		strcpy(name, name_);
		strcpy(iban, iban_);
	}

	void print(std::ostream& os)const {
		os << name << " " << iban << " " << balance << std::endl;
	}

	const char* getName()const {
		return name;
	}

	const char* getIban()const {
		return iban;
	}

	double getBalance()const {
		return balance;
	}

private:
	char name[15] = "";
	char iban[30] = "";
	double balance = 0.0;
};

class Bank {
public:
	Bank() = default;
	void init(const char* filename) {
		std::ifstream ifs(filename, std::ios::binary);
		if (!ifs.is_open()) {
			throw std::invalid_argument("KUR");
		}

		size = getFileSize(ifs)/sizeof(BankAccount);
		accounts = new BankAccount * [size];
		ifs.read((char*)accounts, getFileSize(ifs));
		ifs.close();
	}

	void add(const BankAccount& other) {
		if (size == capacity) {
			//resize(capacity * 2);
		}

//		accounts[size++] = new BankAccount(other.getName(), other.getIban(), other.getBalance());
		accounts[size++] = new BankAccount(other);
	}

	void remove(const char* iban) {
		for (int i = 0; i < size; i++) {
			if (strcmp(iban, accounts[i]->getIban()) == 0) {
				delete accounts[i];
				accounts[i] = nullptr;
			}
		}
	}


private:
	BankAccount** accounts = nullptr;
	size_t size = 0;
	size_t capacity = 8;

	size_t getFileSize(std::istream& is)const {
		size_t pos = is.tellg();
		is.seekg(0, std::ios::end);
		size_t res = is.tellg();
		is.seekg(pos);

		return res;
	}
};

