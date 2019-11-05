#ifndef __FSFILELIST_H__
#define __FSFILELIST_H__

//#define LSHTML_COMPAR_DATATIME		// datatime으로 sort, 주석을 하면 name으로 sort
#define LSHTML_FILES_PER_PAGE		100

extern char *enx_webls_get_html_page(char *path, UINT *allocsize, int lspage);
extern char *enx_webls_get_html(char *path, UINT *allocsize);
extern char *enx_webls_get_html_sdcheck(UINT *allocsize);
extern char *enx_webls_get_xml(char *path, UINT *allocsize);
extern char *enx_webls_get_xml_sdcheck(UINT *allocsize);

extern int gWebls_page;

#endif /* __FSFILELIST_H__ */
