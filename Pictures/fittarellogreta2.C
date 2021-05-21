{
 ifstream sc("daterelli2.dat");

  TH1D *hist=new TH1D("hist","",50,0,10);
  
  double a,b;
while(sc>>a>>b)
{
	hist->Fill(b);
}

//fit dell'istogramma 6 composto da breit-wigner e parabola
TF1 * fun1= new TF1("fun1","[0]/(pow(x-[1],2)+pow([2]/2,2))", 3,5);      //((x-[2])*(x-[3])+[1]/4)[0]) altra forma della breit
fun1->SetParameters(1,1,1,1);
fun1->SetParNames("f","g","h","i");
fun1->SetLineColor(3);
TF1 * fun2= new TF1("fun2","[0]*x*x+[1]*x+[2]", 6,10);
fun2->SetParameters(1,1,1);
fun2->SetParNames("l","m","n");

TCanvas * pippo = new TCanvas("pippo","",600,500);

hist->Draw();
//cloniamo l'istogramma di lavoro in modo da conservare una copia pulita
//non sporcata dai processi di fit (è solo una finezza grafica)
TH1D *pino = (TH1D*)hist->Clone();
//effettuiamo i fit delle funzioni componenti
hist->Fit("fun1","R");
hist->Fit("fun2","R+");
//è FONDAMENTALE definire la somma dopo aver fatto il fit con fun1 e fun2
//sum eredita i parametri dei fit preliminari automaticamente
TF1 * sum = new TF1("sum","fun1+fun2", 0, 10);
sum->SetLineColor(1);
//fit globale con la funzione somma
hist->Fit("sum","R+");
//contenitore per copiare i parametri dopo il fit globale
double parametri[6];
//metodo per copiare in un solo colpo tutti i parametri
sum->GetParameters(parametri);
//sum->GetParameters(&parametri[0]); //comando equivalenti, in questo caso specifichiamo da quale elemento deve partire a copiare

//IMPORTANTISSIMO->Correggiamo l'intervallo di definizione delle funzioni componenti
fun1->SetRange(0,10);
fun2->SetRange(0,10);
//passiamo alle componenti i parametri ottenuti con il fit globale
fun1->SetParameters(&parametri[0]);
fun2->SetParameters(&parametri[3]);

pino->Draw();

fun1->Draw("same");
fun2->Draw("same");

grafico->SaveAs("fitsecondo.pdf");

}
