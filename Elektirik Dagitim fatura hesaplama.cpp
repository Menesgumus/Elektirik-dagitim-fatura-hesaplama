#include <iostream>
#include <string>
#include <fstream>                  //kutuphaneleri ekliyoruz
#include <sstream>
#include <string.h>

using namespace std;

   class Abone{                       //abone class'i olusturuyoruz
        
    protected:
    
    int ID;
    string Ad;
    string Soyad;
    int tuketim_miktari;
    int tuketim_limiti;                     //protected olarak degerleri belirliyoruz
    float fatura_tutari;      
    int limit_asim_miktari;  
    string kurum_adi;
    string abone_tipi;
    
    public: 
    
    Abone(int ID, string Ad ,string Soyad ,int tuketim_miktari , string abone_tipi , int tuketim_limiti = 150)        
    : ID(ID), Ad(Ad), Soyad(Soyad), tuketim_miktari(tuketim_miktari), tuketim_limiti(tuketim_limiti), abone_tipi(abone_tipi) {}     //Burada "this->" operatoru de kullanilabilir
    
    
    float fatura_hesapla(int tuketim_miktari , string abone_tipi) {     //fatura hesaplama fonkisonunu 2 tane parametre alacak sekilde yaziyoruz
        
        if  (abone_tipi == "M"){

            if  (tuketim_miktari <= tuketim_limiti) {
                fatura_tutari = tuketim_miktari * 1.4;                 //yeni fatura tutarini olusturuyor
            } 
            else {
                limit_asim_miktari = (tuketim_miktari - tuketim_limiti);
                fatura_tutari = 210 + limit_asim_miktari * 2.4 ;         // 150 * 1.4  =210 tl yapiyor. (maksimum limit degeri icin)
            }
        }  

        else if (abone_tipi =="S"){

           fatura_tutari = tuketim_miktari * 0.4;
        }

        else if (abone_tipi =="T"){
                                                                    //ayni islemleri abone tipine gore duzenliyoruz 
            fatura_tutari = tuketim_miktari * 1.1;
        }
        
        return fatura_tutari;
    
    }


    virtual char *abone_tipini_getir(char * abone_tipi) = 0;            //pure virtual fonksiyon olusturuyoruz
   
    virtual void yazdir() {
        
        cout <<endl<<"ID: "<< ID <<"  "<<"Ad: "<< Ad  <<"  "<<"Soyad: "<< Soyad <<"  "<<"tuketim miktari: "<< tuketim_miktari  <<"  "<<"Kurum adi: "<< kurum_adi <<"  "<<"fatura tutari: "<<"  "<< fatura_tutari << endl<<endl;
   }                                    //yazdir fonksiyonda parametrelerini bastiriyoruz ve araya bosluk birakiyoruz
};

    class Mesken :public Abone {       //abone classini public olarak kalitim alan Mesken classi olusturuyoruz
     
     private:
    
        int limit_asim_miktari;         
     
     public:
        Mesken(int ID, string Ad, string Soyad, int tuketim_miktari,string abone_tipi ="M", int tuketim_limiti = 150)
         : Abone(ID, Ad, Soyad, tuketim_miktari, abone_tipi, tuketim_limiti), limit_asim_miktari(tuketim_miktari - tuketim_limiti) {}

     
    char *abone_tipini_getir(char *abone_tipi) override {
        return abone_tipi;
    }
     
     virtual void fatura_hesapla(int tuketim_miktari , char *abone_tipi )  {}

    };//mesken sinifinda abone sinifindan farkli bir parametre almadigi icin direkt abone icin kullandigimiz her seyi burda da dondurebiliyoruz

    class Kurumsal : public Abone {         //abone classini public olarak kalitim alan kurumsal classini olusturuyoruz

        private:    

        string kurum_adi;           //abone classinda olmayan kurum adi parametresini isliyoruz
    
        public:
        
        Kurumsal(int ID, string Ad, string Soyad, int tuketim_miktari, string kurum_adi, string abone_tipi = "T", int tuketim_limiti = 150)

         : Abone(ID, Ad, Soyad, tuketim_miktari, abone_tipi, tuketim_limiti), kurum_adi(kurum_adi) {}


         char* abone_tipini_getir(char* abone_tipi) override {
            if (strcmp(abone_tipi, "S") == 1) {

                return "Sanayi";
            }
            else  if (strcmp(abone_tipi, "T") == 1) {
                                     
                return "Ticari";

            }
            return "";                  //burada mesken(Sanayi ve Ticari disindaki her sey) gelirse kurum adini bos birakiyor.
        }

        virtual void yazdir() override {
        
        cout <<endl<<"ID: "<< ID <<"  "<<"Ad: "<< Ad  <<"  "<<"Soyad: "<< Soyad <<"  "<<"tuketim miktari: "<< tuketim_miktari  <<"  "<<"Kurum adi: "<< kurum_adi <<"  "<<"fatura tutari: "<<"  "<< fatura_tutari << endl<<endl;

    }     
    
    };


int main() {
    ifstream Aboneler("Aboneler.txt");  //fstream kutuphanesini ekleyip ifstream ile aboneler.txt dosyasini okuyoruz 
  

    if (!Aboneler.is_open()) {
        cerr << "Dosya acilamadi!" << endl;     //dosya acilmaz ise hata mesaji olan "return -1" olan hata mesajini donduruyor
        return -1;
    }
    
    int ID, tuketim_miktari,tuketim_limiti, limit_asim_miktari;
    string abone_tipi, Ad, Soyad, kurum_adi;
    string satir;                               //diger degerlerden  farkli olarak "satir" stringini ekliyoruz. Tum metni tam okuyabilmek icin

    Abone* fatura = nullptr;
        cout <<endl<<endl<<endl<<endl<<endl<<  "MESKEN ABONELERI :" <<"     "<<endl<<endl<<endl<<endl<<endl;         //Mesken abonelerini belli edecek bir baslik atiyoruz
                                                        //baslangicta dosyadan once acilan bir taslak yaziliyor
     while (getline(Aboneler, satir)) {             //getline fonksiyonu ile txt dosyasinin tum satirlarini okuyoruz
        stringstream ss(satir);            
        
        ss >> abone_tipi >> ID >> Ad >> Soyad >> tuketim_miktari;       //bosluklara gore verileri ayiriyoruz

        Abone* fatura = nullptr;        //bos bir pointer atiyoruz
    
         if (abone_tipi == "M") {
            int tuketim_limiti = 150; 
            fatura = new Mesken(ID, Ad, Soyad, tuketim_miktari, abone_tipi, tuketim_limiti);     
            fatura->Abone::fatura_hesapla(tuketim_miktari,abone_tipi);                             //meskenin faturasini hesapliyor
            fatura->Abone::yazdir();                                                        //diger yazdir fonksiyonlarini gormezden gelerek abone classindaki yazdir fonksiyonunu calistiriyor
        } 
     } 
            Aboneler.clear();
            Aboneler.seekg(0);

        cout <<endl<<endl<<endl<<endl<<endl<<  "SANAYI ABONELERI :" <<"     "<<endl<<endl<<endl<<endl<<endl;          //Sanayi abonelerini belli edecek bir baslik atiyoruz

         while (getline(Aboneler,satir)) {
                stringstream ss(satir);    
                    
                ss >> abone_tipi >> ID >> Ad >> Soyad >> tuketim_miktari;    
                 
                
                if (abone_tipi == "S"){
                    getline(ss, kurum_adi);  
                    kurum_adi = kurum_adi.substr(1);    //kurum adini alirken "substr(1)" kullanarak 1 kelimeden fazla olan isimleri de kurum adina yazabiliyoruz
                    fatura = new Kurumsal(ID, Ad, Soyad, tuketim_miktari, kurum_adi, abone_tipi);  //kurumsalin faturasini hesapliyor
                    fatura->Abone::fatura_hesapla(tuketim_miktari,abone_tipi);
                    fatura->yazdir();
                }
                }
            Aboneler.clear();
            Aboneler.seekg(0);
        
        cout <<endl<<endl<<endl<<endl<<endl<<  "TICARI ABONELERI :" <<"     "<<endl<<endl<<endl<<endl<<endl;        // //Ticari abonelerini belli edecek bir baslik atiyoruz
         while (getline(Aboneler,satir)) {
                stringstream ss(satir);    
                    
                ss >> abone_tipi >> ID >> Ad >> Soyad >> tuketim_miktari;    
                 
                
                if (abone_tipi == "T"){
                    getline(ss, kurum_adi);  
                    kurum_adi = kurum_adi.substr(1);    //kurum adini alirken "substr(1)" kullanarak 1 kelimeden fazla olan isimleri de kurum adina yazabiliyoruz
                    fatura = new Kurumsal(ID, Ad, Soyad, tuketim_miktari, kurum_adi, abone_tipi);  //kurumsalin faturasini hesapliyor
                    fatura->Abone::fatura_hesapla(tuketim_miktari,abone_tipi);
                    fatura->yazdir();
                }
                }   
            delete(fatura);

        
    Aboneler.close();   //aboneler.txt dosyasini kapatiyor
        
          return 0;
} 
                                        // NOT KISMI //

/* Program calisirken bir uyari veriyor(programin calismasina engel olan bir sey degil sadece char olarak kullandigim bir hesaplamada string kullandigim icin oluyor).

tum faturalari dogru bir sekilde hesaplayip isliyor

isi bitince bellekten siliyor

bazi ifadeleri daha gormedik ama projemde kullanmak icin arastirarak buldum ve ekledim. (seekg ,substr gibi)
*/