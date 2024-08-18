# Progetto di algoritmi e principi dell'informatica 2023

## _Specifica_
Si consideri un’autostrada descritta come una sequenza di stazioni di servizio. Ogni stazione di servizio si trova 
ad una distanza dall’inizio dell’autostrada espressa in chilometri da un numero intero positivo o nullo. Non esistono
due stazioni di servizio aventi la stessa distanza: ogni stazione di servizio è quindi univocamente identificata dalla
sua distanza dall’inizio dell’autostrada.
Ogni stazione di servizio è dotata di un parco veicoli elettrici a noleggio. Ogni veicolo è contraddistinto
dall’autonomia data da una carica delle batterie, espressa in chilometri, da un numero intero positivo. Il parco
veicoli di una singola stazione comprende al più 512 veicoli. Presa a noleggio un’auto da una stazione s, è possibile
raggiungere tutte quelle la cui distanza da s è minore o uguale all’autonomia dell’automobile.
Un viaggio è identificato da una sequenza di stazioni di servizio in cui il conducente fa tappa. Ha quindi inizio
in una stazione di servizio e termina in un’altra, passando per zero o più stazioni intermedie. Si assuma che il
conducente non possa tornare indietro durante il viaggio, e noleggi un’auto nuova ogniqualvolta faccia tappa in
una stazione di servizio. Quindi, date due tappe consecutive s e t, t deve essere sempre più lontana dalla partenza
rispetto a s, e t deve essere raggiungibile usando uno dei veicoli disponibili in s.
L’obiettivo del progetto è il seguente: data una coppia di stazioni, pianificare il percorso con il minor numero
di tappe tra di esse. Nel caso siano presenti più percorsi con lo stesso numero minimo di tappe (cioè a pari merito),
deve essere scelto il percorso che predilige le tappe con distanza più breve dall’inizio dell’autostrada.

#### Comandi
I comandi accettati dal programma, con i relativi attributi e output, sono i seguenti:
| Comando            | Attributi                                                  | Output                            |
| ------------------ | ---------------------------------------------------------- | --------------------------------- |
| aggiungi-stazione  | distanza numero-auto autonomia-auto-1 ... autonomia-auto-n | aggiunta/non aggiunta             |
| demolisci-stazione | distanza                                                   | demolita/non demolita             |
| aggiungi-auto      | distanza-stazione autonomia-auto-da-aggiungere             | aggiunta/non aggiunta             |
| rottama-auto       | distanza-stazione autonomia-auto-da-rottamare              | rottamata/non rottamata           |
| pianifica-percorso | distanza-stazione-partenza distanza-stazione-arrivo        | nessun percorso/*lista di interi* |

Il funzionamento di ogni comando con eventuali esempi è riportato di seguito: 
* aggiungi-stazione <br>
  Aggiunge una stazione, posta alla distanza indicata, dotata di numero-auto, dall’autonomia indicata.
* demolisci-stazione <br>
  Rimuove la stazione posta alla distanza indicata, se essa esiste.
* aggiungi-auto <br>
  Se la stazione esiste, aggiunge un’auto alla stessa. È possibile avere più auto con la stessa autonomia.
* rottama-auto <br>
  Rimuove un’automobile dalla stazione indicata, se la stazione esiste ed è dotata di almeno un’automobile con l’autonomia indicata.
* pianifica-percorso <br>
  Richiede di pianificare il percorso con i vincoli sopra indicati. 
  Stampa attesa come risposta: le tappe in ordine di percorrenza, rappresentate con la distanza delle stazioni
  da inizio autostrada, separate da spazi e alla fine seguite da un a-capo. Devono essere incluse partenza e
  arrivo; se coincidono la stazione viene stampata una sola volta. Se il percorso non esiste, stampa nessun
  percorso. L’azione di pianificazione non altera le stazioni o il loro parco veicoli. Le stazioni date sono
  sicuramente presenti. <br>

#### Esempio
Di seguito un esecuzione d'esempio della specifica richiesta:
| Testo in ingresso (stdin)      | Testo in uscita (stdout)                       | Commento                            |
| ------------------------------ | ---------------------------------------------- | ----------------------------------- |
| aggiungi-stazione 20 3 5 10 15 | aggiunta                                       | *Aggiunta stazione km 20*           |
| aggiungi-stazione 4 3 1 2 3    | aggiunta                                       | *Aggiunta stazione km 4*            |
| aggiungi-stazione 30 0         | aggiunta                                       | *Aggiunta stazione km 30*           |
| demolisci-stazione 3           | non demolita                                   | *Non esiste stazione km 3*          |
| demolisci-stazione 4           | demolita                                       | *Demolita stazione km 4*            |
| pianifica-percorso 30 20       | nessun percorso                                | *Non esiste percorso da 30*         |
| aggiungi-auto 30 40            | aggiunta                                       | *Aggiunta auto con autonomia 40*    |
| aggiungi-stazione 50 3 20 25 7 | aggiunta                                       | *Aggiunta stazione km 50*           |
| rottama-auto 20 8              | non rottamata                                  | *Non esiste auto con autonomia 8*   |
| rottama-auto 9999 5            | non rottamata                                  | *Non esiste stazione km 9999*       |
| rottama-auto 50 7              | rottamata                                      | *Rottamata auto con autonomia 7*    |
| pianifica-percorso 20 30       | 20 30                                          | *Usa auto con autonomia 15*         |
| pianifica-percorso 20 50       | 20 30 50                                       | *Usa auto con autonomia 15 e 40*    |
| pianifica-percorso 50 30       | 50 30                                          | *Usa auto con autonomia 25*         |
| pianifica-percorso 50 20       | 50 30 20                                       | *Usa auto con autonomia 25 e 40*    |
| aggiungi-auto 50 30            | aggiunta                                       | *Aggiunta auto autonomia 30*        |
| pianifica-percorso 50 20       | 50 20                                          | *Usa auto con autonomia 30 e 40*    |
