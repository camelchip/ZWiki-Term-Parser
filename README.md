## ZWiki Term Parser
### Inputs
* `Term Translations Input.csv` - CSV file containing translations for various Terms.
  * Each column represents a language or Latinization of a language, using the format: <br />`US-en | EU-fr | US-fr | EU-de | US-es | EU-es | EU-it | EU-nl | EU-ru | EU-ru (Latin) | JP-ja | JP-ja (Latin) | TW-zh | TW-zh (Latin) | CN-zh | CN-zh (Latin) | KR-ko | KR-ko (Latin)`
  * Note that these language codes are based on those used in BotW, but are auto-converted to the lang codes used on the Wiki.
  * Translations for non-Latin languages require Romanization. Those that don't have such counterparts are currently skipped and not added to the Data page output. A log of these is listed in `errLogRomanization.txt`
* `translationPage.txt` - Text file containing the whole contents of a Translation page from the Wiki (i.e. Data:Translations namespace).
  * The relevant {{Translation/Store}} entries for each Term will be populated based on translations provided in the CSV.
  * If a translation already exists for a given langauge, but is different from the translation in the CSV, said translation will not be overwritten within the entry. A log of such discrepancies is listed in `errLog.txt` 
  
### Outputs
* `translationPage_output.txt` - Text file containing the updated contents of the Translation page.
  * Since the input was based on the current Data page (in `translationPage.txt`), this updated version is ready to be copy/pasted onto the Wiki. Check the diff before publishing the changes, though!
* `errLog.txt` - Text file logging discrepencies between the current translations (from the Data page) and the new translations (from the CSV).
  * For example, the US-es entry for `Soldier's Claymore` might be `Espada grande de soldado` on the Data page, but the CSV lists a translation of `Mandoble de soldado`.
  * This is logged as `-Soldier's Claymore (esL): 'Espada grande de soldado' vs. 'Mandoble de soldado'`.
* `errLogRomanization.txt` - Text file logging translations that were skipped due to not having a Romanized entry in the CSV.
  * For example, the KR-ko entry for `Soldier's Claymore` might have a translation as `병사의 양손검`, but not have a corresponding Romanization.
  * This is logged as `-(ko) Soldier's Claymore: no Romanization available (병사의 양손검 == '')`.
