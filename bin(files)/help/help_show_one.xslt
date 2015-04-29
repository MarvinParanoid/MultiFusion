<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:variable name="pluginDocument" select="document('plugins.xml')" />

<xsl:template match="/">
<html>
<head>
	<style>
		.link {
			cursor: pointer;
			color: blue;
			text-decoration: underline;
		}
	</style>
	<script>
		function show(id) {
			document.getElementById(id).style.display = "block";
			document.getElementById('list').style.display = "none";
		}
		function contents(id) {
			document.getElementById(id).style.display = "none";
			document.getElementById('list').style.display = "block";
		}
		function switchTo(obj, id) {
			document.getElementById(id).style.display = "block";
			obj.style.display = "none";
		}
	</script>
</head>
<body>
	<xsl:call-template name="lst"/>
	<xsl:call-template name="content"/>
</body>
</html>
</xsl:template>

<xsl:template name="lst">
<div id="list" style="display: none;">
<div onclick="show('Common')" class="link">
	<xsl:value-of select="/plugins/Common/Caption"/>
</div>
<xsl:for-each select="/plugins/plugin">
<xsl:variable name="currId" select="Id"/>
<xsl:if test="$pluginDocument/plugins/plugin[./Id=$currId]/@included='true'">
	<div>
		<xsl:attribute name="onclick">show('<xsl:value-of select="Id" />')</xsl:attribute>
		<xsl:attribute name="class">link</xsl:attribute>
		<xsl:value-of select="Caption"/>
	</div>
</xsl:if>
</xsl:for-each>
</div>
</xsl:template>

<xsl:template name="content">
<div id="Common" style="display: none;">
	<div onclick="contents('Common')" class="link" style="float: right">
		<span>В оглавление...</span>
	</div>
	<b><xsl:value-of select="/plugins/Common/Caption"/></b><br/>
	<xsl:copy-of select="/plugins/Common/Help/*|/plugins/Common/Help/text()"/>
</div>
<xsl:for-each select="/plugins/plugin">
<xsl:variable name="currId" select="Id"/>
<xsl:if test="$pluginDocument/plugins/plugin[./Id=$currId]/@included='true'">
	<div>
		<xsl:attribute name="id"><xsl:value-of select="Id" /></xsl:attribute>
		<xsl:choose>
			<xsl:when test="Id/text()='%helpId%'">
				<xsl:attribute name="style">display: block;</xsl:attribute>
			</xsl:when>
			<xsl:otherwise>
				<xsl:attribute name="style">display: none;</xsl:attribute>
			</xsl:otherwise>
		</xsl:choose>
		<div>
			<xsl:attribute name="onclick">contents('<xsl:value-of select="Id" />')</xsl:attribute>
			<xsl:attribute name="class">link</xsl:attribute>
			<xsl:attribute name="style">float: right;</xsl:attribute>
			<span>В оглавление...</span>
		</div>
		<b><xsl:value-of select="Caption"/></b><br/>
		<xsl:copy-of select="Help/*|Help/text()"/>
		<br/>
		<br/>
		<xsl:for-each select="SeeAlso/Id">
			<xsl:call-template name="seeAlso">
				<xsl:with-param name="alsoId" select="text()"/>
			</xsl:call-template>
		</xsl:for-each>
	</div>
</xsl:if>
</xsl:for-each>
</xsl:template>

<xsl:template name="seeAlso">
	<xsl:param name="alsoId" />
	<xsl:if test="$pluginDocument/plugins/plugin[./Id=$alsoId]/@included='true'">
		<div>
			<xsl:attribute name="class">link</xsl:attribute>
			<xsl:attribute name="onclick">switchTo(this.parentNode, '<xsl:value-of select="$alsoId" />')</xsl:attribute>
			<xsl:value-of select="/plugins/plugin/Caption[../Id=$alsoId]"/>
		</div>
	</xsl:if>
</xsl:template>

</xsl:stylesheet>