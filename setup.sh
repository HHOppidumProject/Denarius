#! /bin/sh
ScriptName=$0
Help(){
	printf 'Script for installing tools to help with development of the Hellenic Hospitality Oppidum currency, the Solidus.\n\n'
	
	printf 'Syntax: '$ScriptName' [-h|-VSC|-VS|-vcpkg|-njs|-f]\n'
	printf "options:\n"
	printf '\th\t\tPrints this help function\n\t\tAlias\t-? | --help\n'
	printf '\tVSC\t\tInstalls Visual Studio Code [y/n] (cannot be used with -VS)\n\t\tAlias\t--VSCode | --Visual-Studio-Code\n'
	printf '\tVS\t\tInstalls Visual Studio Community Edition [y/n] (cannot be used with -VSC)\n\t\tAlias\t--VSCommunity | --Visual-Studio-Community\n'
	printf '\tvcpkg\t\tInstalls vcpkg package manager [y/n]\n\t\tAlias\t--vcp\n'
	printf '\tvcpkgPath\t\tPath to install vcpkg into \n\t\tAlias\t--vcpkgInstallPath| --vcpp\n'
	printf '\tnjs\t\tInstalls Node.JS [y/n]\n\t\tAlias\t--NodeJS\n'
	printf '\tf\t\tForces execution\n\t\tAlias\t-F | --force\n'
}

VSCode=false
VSCommunity=false
vcpkg=true
njs=true
force=false
vcpkgPath=""

while [[ $# -gt 0 ]]; do
	key="$1"
	
	case $key in 
		-h|--help|-\? )
			Help
			shift
			shift
			exit ;;
			
		-VSC|--VSCode|--Visual-Studio-Code )
			if [[ "$2" == true ]]; then
				VSCode=true
			elif [[ "$2" == "true" ]]; then
				VSCode=true
			elif [[ "$2" == "y" ]]; then
				VSCode=true
			elif [[ "$2" == false ]]; then
				VSCode=false
			elif [[ "$2" == "false" ]]; then
				VSCode=false
			elif [[ "$2" == "n" ]]; then
				VSCode=false
			elif [[ "$2" == "" ]]; then
				VSCode=true
			elif [[ "$2" == " " ]]; then
				VSCode=true
			else
				printf '\"'$2'\" unexpected value for '$1', ignoring... and setting to false\n'
			fi
			shift
			shift
			;;
			
		-VS|--VSCommunity|--Visual-Studio-Community )
			if [[ "$2" == true ]]; then
				VSCommunity=true
			elif [[ "$2" == "true" ]]; then
				VSCommunity=true
			elif [[ "$2" == "y" ]]; then
				VSCommunity=true
			elif [[ "$2" == false ]]; then
				VSCommunity=false
			elif [[ "$2" == "false" ]]; then
				VSCommunity=false
			elif [[ "$2" == "n" ]]; then
				VSCommunity=false
			elif [[ "$2" == "" ]]; then
				VSCommunity=true
				shift
				continue
			elif [[ "$2" == " " ]]; then
				VSCommunity=true
				shift
				continue
			else
				printf '\"'$2'\" unexpected value for '$1', ignoring... and setting to false\n'
			fi
			shift
			shift
			;;
			
		-vcpkg|--vcp )
			if [[ "$2" == true ]]; then
				vcpkg=true
			elif [[ "$2" == "true" ]]; then
				vcpkg=true
			elif [[ "$2" == "y" ]]; then
				vcpkg=true
			elif [[ "$2" == false ]]; then
				vcpkg=false
			elif [[ "$2" == "false" ]]; then
				vcpkg=false
			elif [[ "$2" == "n" ]]; then
				vcpkg=false
			elif [[ "$2" == "" ]]; then
				vcpkg=true
			elif [[ "$2" == " " ]]; then
				vcpkg=true
			else
				printf '\"'$2'\" unexpected value for '$1', ignoring... and setting to true\n'
			fi
			shift
			shift
			;;
			
		-vcpkgPath|--vcpkgInstallPath|--vcpp )
			vcpkgPath="$2"
			shift
			shift
			;;
			
		-njs|--NodeJS )
			if [[ "$2" == true ]]; then
				njs=true
			elif [[ "$2" == "true" ]]; then
				njs=true
			elif [[ "$2" == "y" ]]; then
				njs=true
			elif [[ "$2" == false ]]; then
				njs=false
			elif [[ "$2" == "false" ]]; then
				njs=false
			elif [[ "$2" == "n" ]]; then
				njs=false
			elif [[ "$2" == "" ]]; then
				njs=true
			elif [[ "$2" == " " ]]; then
				njs=true
			else
				printf '\"'$2'\" unexpected value for '$1', ignoring... and setting to true\n'
			fi
			shift
			shift
			;;
			
		-f|-F|--force )
			force=true
			shift
			;;
			
		* ) 
			Help
			exit;;
	esac
done

echo "VSCode = ${VSCode}"
echo "VSCommunity = ${VSCommunity}"
echo "vcpkg = ${vcpkg}"
echo "njs = ${njs}"
echo "force = ${force}"

if [[ "$force" == false ]]; then
	if [[ "$VSCode" == true ]] && [[ "$VSCommunity" == true ]]; then
		echo "Cannot have both -VSC and -VS set to true"
		echo
		Help
		exit
	elif [[ "$vcpkg" == true ]] && [[ "$vcpkgPath" = "" ]]; then
		echo "Please set the path to install vcpkg into"
		Help
		exit
	fi
fi

if [[ "$VSCode" == true ]]; then
	echo "Getting and installing Visual Studio Code"
	curl "https://code.visualstudio.com/sha/download?build=stable&os=win32-x64" -o VSCode-Installer.exe -L
	./VSCode-Installer.exe
	wait $!
else
	echo "Skipping instaling Visual Studio Code"
fi

if [[ "$VSCommunity" == true ]]; then
	echo "Getting and installing Visual Studio Community Edition"
	curl "https://download.visualstudio.microsoft.com/download/pr/22c17f05-944c-48dc-9f68-b1663f9df4cb/2492827d2bc58c5cd0a4e4ab40da86910d05ee96f71d69b561457885b3249a91/vs_Community.exe" -o VS-Community-Installer.exe -L
	./VS-Community-Installer.exe
	wait $!
else
	echo "Skipping installing Visual Studio Community Edition"
fi

if [[ "$vcpkg" == true ]]; then
	echo "Getting vcpkg and installing to ${vcpkgInstallPath}"
	git clone https://github.com/microsoft/vcpkg $vcpkgPath
	powershell '$([Environment]::SetEnvironmentVariable("PATH", [Environment]::GetEnvironmentVariable("PATH") + ";$vcpkgPath", "Machine"))'
	./$vcpkgPath/vcpkg.exe integrate install
else
	echo "Skipping installing vcpkg"
fi

if [[ "$njs" == true ]]; then
	echo "Getting and installing Node.JS"
	curl https://nodejs.org/dist/v14.17.6/node-v14.17.6-x64.msi -o nodejs-installer.msi -L
	powershell "\$njsins = Start-Process \"./nodejs-installer.msi\" -PassThru ; \$njsins.WaitForExit()"
else
	echo "Skipping installation of Node.JS"
fi
	
git -C $(pwd) rev-parse
RC=$?

if [[ $RC -eq 0 ]]; then
	git checkout origin/dev
	git pull
else
	echo "Not in a git repository"
fi

rm VSCode-Installer*
rm VS-Community-Installer*
rm nodejs-installer*