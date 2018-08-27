#include "mecab.h"
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <list>


namespace Kama{

	typedef struct _Node {
        unsigned int index;
		unsigned int id;
		unsigned int nbest;
		char* surface;
		char* feature;
	} Node;
	
	class Tagger{

		private:
			MeCab::Tagger* mcTagger = NULL;	
			MeCab::Lattice* mcLattice = NULL;
			std::set<std::string> stopwordList;
			Node parseMeCabNode(const MeCab::Node* node, unsigned int nbest);
            const char* defaultDicPath = "/usr/local/lib/mecab/dic/mecab-ko-dic";

		public:
			Tagger(const char* userDicPath){
                std::string dicPath;
                dicPath.append(" -d ");
                dicPath.append(userDicPath);
				this->mcTagger = MeCab::createTagger((const char*)dicPath.c_str());
				this->mcLattice = MeCab::createLattice();
				CHECK_ERROR(this->mcTagger);
			};

			~Tagger(){
				if(!this->mcTagger){
					delete this->mcTagger;
					this->mcTagger = NULL;
				}
				if(!this->mcLattice){
					delete this->mcLattice;
					this->mcLattice = NULL;
				}
				this->clearStopwordList();
			}

			std::vector<Kama::Node>* parse(const char* str);
			
			std::vector<Kama::Node>* parse(const char* str, unsigned int nbest);

			void setStopwordList(std::list<const char*> stopwordList);

			void clearStopwordList();

			void addStopword(const char* stopword);

			void printStopword();

			bool isStopword(const char* word);

			inline void CHECK_ERROR(const void *eval){
				if(!eval){
					const char *e = this->mcTagger ? this->mcTagger->what() : MeCab::getLastError();
					std::cerr << "tagger_exception : " << e << std::endl;
					delete this->mcTagger;
					this->mcTagger = NULL;
				}
			};

	};
	
}
