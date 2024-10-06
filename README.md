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
* `errLogRomanization.txt` - Text file logging translations that were skipped due to not having a Romanized entry.
