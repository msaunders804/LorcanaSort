#include <C:\Users\User\source\repos\Lorcana\Lorcana\Lorcana.h>

void PerceptHashInput(Mat& img)
{
	Mat gray, resized, hash;
	cv::cvtColor(img, gray, COLOR_BGR2GRAY);
	cv::resize(gray, resized, Size(128, 128));
	img_hash::pHash(resized, hash);
	//hash = HextoMatHash(hash1, hash.rows);
	cout << hash <<endl;
	CompareHashes(hash);
}

void CompareHashes(const Mat& inputHash)
{
	try {
		sql::mysql::MySQL_Driver* driver;
		sql::Connection* con;
		driver = sql::mysql::get_mysql_driver_instance();
		con = driver->connect("tcp://localhost:3306",
			"root", "poiLKJmnb147852");
		con->setSchema("lorcanaref");
		sql::Statement* stmt = con->createStatement();

		// Execute the query to retrieve phash values from the card table
		sql::ResultSet* res = stmt->executeQuery("SELECT phash, name FROM card");
		int closeMatch = numeric_limits<int>::max();
		string closestName;
		string closestHash;

		// Process the results and compare with the local phash
		if (res != nullptr) {
			while (res->next()) {
				// Retrieve the phash value from the database
				string dbPhashValue = res->getString("phash");
				string dbNameValue = res->getString("name");
				int i = 0;
				while (i < dbPhashValue.length())
				{
					if (dbPhashValue[i] == '[' || dbPhashValue[i] == ']' || dbPhashValue[i] == ' ')
					{
						dbPhashValue.erase(i, 1);
					}
					else {
						i++;
					}
				}
				string input = dbPhashValue;
				istringstream ss(input);
				vector<int> dbhashvec;
				string token;
				while (getline(ss, token, ',')) {
					int num;
					istringstream(token) >> num;
					dbhashvec.push_back(num);
				}

				//Generate vector for input
				std::vector<int>inputVector = inputHash.reshape(1, 1);
				// Compare with the local phash
				int compareSum;
				compareSum = HammingDistance(dbhashvec, inputVector);
				cout << "Difference: " << compareSum << "Name: " << dbNameValue << endl;
				if (compareSum < closeMatch) {
					closeMatch = compareSum;
					closestName = dbNameValue;
					closestHash = dbPhashValue;
				}
			}
			
				if (!closestName.empty()) {
					cout << "Closest match: " << closestName << " with Hamming distance: " << closeMatch << endl;
					cout << "Hash " << closestHash << endl;
			}
			delete res;
			delete stmt;
			delete con;
		}
	}
	catch (sql::SQLException& e)
	{
		cerr << "SQL Error: " << e.what() << endl;
	}

}
int HammingDistance(vector<int>& dbHash, vector<int>& inputHash) {
	//DO hamming of each elemt
	//Find difference of each elemt --> Sum all elements look for the smallest?
	int sum = 0;
	if (dbHash.size() != inputHash.size())
	{
		cout << "Error in hashing. Hashes are not the same size" << endl;
	}
	for (int i = 0; i < inputHash.size(); i++)
	{
		sum += abs(inputHash[i] - dbHash[i]);
	}
	return abs(sum);
}
void PerceptHashForDB()
{
	string cardnum, dir, card, filename;
	Mat gray, resized, hash;
	cout << "What is the directory your images are stored? Note:Please name your images based on the set Number and card number ie 2-123" << endl;
	cin >> dir;
	remove("C:\\Users\\User\\Documents\\LorcanaCardImages\\Hashes.csv");
	for (int i = 1; i <= 3; i++) {
		for (int j = 1; j <= 216; j++) {
			filename = dir + '\\' + to_string(i) + '-' + to_string(j) + ".jpg";
			ifstream file(filename);
			if (file.is_open()) {
				cardnum = to_string(i) + '-' + to_string(j);
				Mat img = imread(filename, IMREAD_COLOR);
				cv::cvtColor(img, gray, COLOR_BGR2GRAY);
				cv::resize(gray, resized, Size(128, 128));
				img_hash::pHash(resized, hash);
				file.close();
				AddtoFile(cardnum, hash);
			}
		}
	}
}

void AddtoFile(string cardnum, Mat hash)
{
	ofstream LorHashes("C:\\Users\\User\\Documents\\LorcanaCardImages\\Hashes.csv", std::ios::app);
	LorHashes << cardnum << '\t' << hash << endl;
	LorHashes.close();
}

void AddtoDB()
{
	try {
		sql::mysql::MySQL_Driver* driver;
		sql::Connection* con;
		driver = sql::mysql::get_mysql_driver_instance();
		con = driver->connect("tcp://localhost:3306",
			"root", "poiLKJmnb147852");
		con->setSchema("lorcanaref");

		ifstream inputFile("C:\\Users\\User\\Documents\\LorcanaCardImages\\Hashes.csv");
		if (!inputFile.is_open())
		{
			cerr << "Error opening CSV" << endl;
			return;
		}

		string line;
		while (getline(inputFile, line)) {
			istringstream iss(line);
			string setnum, phash;
			if (!(std::getline(iss, setnum, '\t') && std::getline(iss, phash, '\t'))) {
				cerr << "Error Parsing CSV" << endl;
				continue;
			}
			cout << setnum << "," << phash << endl;
			string updateQuery = "UPDATE card SET phash = ? WHERE setnum = ?";
			sql::PreparedStatement* pstmt = con->prepareStatement(updateQuery);

			pstmt->setString(1, phash);
			pstmt->setString(2, setnum);

			pstmt->executeUpdate();

			delete pstmt;
		}
	}
	catch(sql::SQLException& e)
	{
		std::cerr << "SQL Error: " << e.what() << std::endl;
	}
}

