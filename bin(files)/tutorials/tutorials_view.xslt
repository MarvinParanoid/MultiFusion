<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
<html>
<head>
</head>
<body>
	<xsl:for-each select="/tutorials/Tutorial">
	<xsl:variable name="currId" select="Id"/>
	<xsl:if test="$currId='%id%'">
		<b><xsl:value-of select="Caption"/></b><br/>
		<xsl:copy-of select="Text/*|Text/text()"/>
	</xsl:if>
</xsl:for-each>
</body>
</html>
</xsl:template>

</xsl:stylesheet>
