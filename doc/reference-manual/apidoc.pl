#!/usr/bin/perl -w
###############################################################################
# This program creates C struct definitions of functions/procedures defined
# in an module declaration xml file
###############################################################################
use 5.008000;
use strict;
use warnings;

use Carp;
use Data::Dumper;
use XML::Simple;

if ( scalar(@ARGV) < 1 )
{
    print STDERR "usage: apidoc.pl [-lang LANG] api.xml\n";
    exit(1);
}
my $input = shift(@ARGV);
my $lang = "en";
if ( $input eq "-lang" )
{
    $lang = shift(@ARGV);
    $input = shift(@ARGV);
}
my $xs = XML::Simple->new();
my $hash = $xs->XMLin($input,
		      KeyAttr => [],
		      KeepRoot => 0,  #strips the <fields> tag without KeepRoot
		      ForceArray => 0,
    ) || croak("XMLin failed: " . $@);

my $module = $hash->{module};

open(OUT, "> apidoc-" . $module . ".xml") || croak("couldn't open output");

print OUT "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
print OUT "<section id=\"$module" . "_funcproc\"><title>Functions and procedures exported by $module</title>\n";

dump_functions($hash);
dump_procedures($hash);
print OUT "</section>\n";
close(OUT);

exit 0;
###########################################################################################################

sub dump_args
{
    my ( $args ) = @_;

    if ( !defined($args) )
    {
	return;
    }

    if ( ref($args) ne 'ARRAY' )
    {
	$args = [$args];
    }

    print OUT "
        <varlistentry>
          <term>arguments</term>
          <listitem>
           <variablelist>";

    foreach my $arg ( @{$args} )
    {
	print OUT "
            <varlistentry>
             <term>" . $arg->{name} . "</term>
             <listitem>
               <simpara>
                type: <link linkend=\"lang_type_" . $arg->{type} . "\">" . $arg->{type} . "</link>
               </simpara>
             </listitem>
            </varlistentry>";
    }

    print OUT "
           </variablelist>
          </listitem>
        </varlistentry>";

}


sub get_args
{
    my ( $args ) = @_;

    if ( !defined($args) )
    {
	return "();";
    }

    if ( ref($args) ne 'ARRAY' )
    {
	$args = [$args];
    }

    my $retval = "(";

    foreach my $arg ( @{$args} )
    {
	$retval .= $arg->{type} . " " . $arg->{name} . ", ";
    }
    chop($retval);
    chop($retval);

    $retval .= ");";

    return $retval;
}



sub dump_functions
{
    my $hash = shift;

    my $funcs = $hash->{function};
    if ( defined($funcs) && (ref($funcs) ne 'ARRAY') )
    {
	$funcs = [$hash->{function}];
    }

    if ( defined($funcs) && (@{$funcs} > 0) )
    {
	print OUT "
<section id=\"$module" . "_functions\"><title>Functions exported by $module</title>
  <variablelist>";

	my %defines;

	foreach my $func ( @{$funcs} )
	{
	    my $desc;
	    if ( defined($func->{description}{$lang}) )
	    {
		$desc = $func->{description}{$lang};
	    }
	    else
	    {
		$desc = $func->{description}{en};
	    }
	    print OUT "
   <varlistentry>
     <term>";
	    if ( ! defined($defines{$func->{name}}) )
	    {
		print OUT "
       <anchor id=\"$module" . "_func_" . $func->{name} . "\"/>";
		$defines{$func->{name}} = 1;
	    }
	    print OUT "
       <function>" . $func->{rettype} . " " . $func->{name} . get_args($func->{arg}) . "</function>
     </term>
     <listitem>
      <variablelist>
        <varlistentry>
         <term>description</term>
         <listitem>
          <simpara>" . $desc . "</simpara>
         </listitem>
        </varlistentry>";
	    dump_args($func->{arg});
	    print OUT "
        <varlistentry>
         <term>return type</term>
         <listitem>
           <simpara>
            <link linkend=\"lang_type_" . $func->{rettype} . "\">" .$func->{rettype} . "</link>
           </simpara>
         </listitem>
        </varlistentry>
      </variablelist>
     </listitem>
   </varlistentry>
";
	}
	
	print OUT "
  </variablelist>
</section>
";
    }
    else
    { # no functions
    }
}



sub dump_procedures
{
    my $hash = shift;

    my $procs = $hash->{procedure};
    if ( defined($procs) && (ref($procs) ne 'ARRAY') )
    {
	$procs = [$hash->{procedure}];
    }

    if ( defined($procs) && (@{$procs} > 0) )
    {
	print OUT "
<section id=\"$module" . "_procedures\"><title>Procedures exported by $module</title>
  <variablelist>";

	my %defines;

	foreach my $proc ( @{$procs} )
	{
	    my $desc;
	    if ( defined($proc->{description}{$lang}) )
	    {
		$desc = $proc->{description}{$lang};
	    }
	    else
	    {
		$desc = $proc->{description}{en};
	    }
	    print OUT "
   <varlistentry>
     <term>";
	    if ( ! defined($defines{$proc->{name}}) )
	    {
		print OUT "
       <anchor id=\"$module" . "_proc_" . $proc->{name} . "\"/>";
		$defines{$proc->{name}} = 1;
	    }
	    print OUT "
       <function>" . $proc->{name} . get_args($proc->{arg}) . "</function>
     </term>
     <listitem>
      <variablelist>
        <varlistentry>
         <term>description</term>
         <listitem>
          <simpara>" . $desc . "</simpara>
         </listitem>
        </varlistentry>";
	    dump_args($proc->{arg});
	    print OUT "
      </variablelist>
     </listitem>
   </varlistentry>
";
	}
	
	print OUT "
  </variablelist>
</section>
";

    }
    else
    { # no procedures
    }
}
