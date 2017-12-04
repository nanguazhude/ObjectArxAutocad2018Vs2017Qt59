#include "UpdateReactor.hpp"

namespace sstd {

	UpdateReactor::UpdateReactor() {

	}	

	namespace {

		class EditorReactor :
			public AcEditorReactor {
		public:
			void databaseConstructed(AcDbDatabase* var) override;
		};

		class DatabaseReacotr : 
			public AcDbDatabaseReactor {
		public:
			DatabaseReacotr() :AcDbDatabaseReactor(){
				acutPrintf(LR"(reacotr added!
)");
			}

			~DatabaseReacotr() {}
					 
			void objectAppended(const AcDbDatabase*  , 
				const AcDbObject* dbObj) override {
				auto varMtext = dynamic_cast< const AcDbMText* >(dbObj);
				if (varMtext) {
					double varBF = 0;
					acutPrintf(LR"(1
)");
					varMtext->getBackgroundScaleFactor(varBF);
					if (std::abs(varBF - 1.005) < 0.000001) {
						return;
					}

					auto var = const_cast<AcDbMText*>(varMtext);
					if (Acad::eOk == var->upgradeOpen()) {
						acutPrintf(LR"(2
)");
						var->setBackgroundFill(true);
						var->setUseBackgroundColor(true);
						var->setBackgroundScaleFactor(1.005);
					}
					else {
						acutPrintf(LR"(3
)");
					}

				}
			}

		};
		
		DatabaseReacotr * globalDatabaseReacotr = nullptr ;
		EditorReactor * globalEditorReactor = nullptr ;
		void EditorReactor::databaseConstructed(AcDbDatabase* var)   {
			var->addReactor(globalDatabaseReacotr);
		}

	}

	extern void loadUpdateReactor() { 
		UpdateReactor::load();
	}
	
	void UpdateReactor::load(){
		arx_add_main_command<UpdateReactor>();
	}
	
	void UpdateReactor::main(){
		globalDatabaseReacotr = new DatabaseReacotr;
		globalEditorReactor = new EditorReactor;
		acedEditor->addReactor(globalEditorReactor);
	}

}/*namepsace sstd*/

