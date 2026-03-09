#! bin/bash
python3 -m venv .i_love_cats
source .i_love_cats/bin/activate
#updrage pip
pip install --upgrade pip
# Install the necessary dependencies
pip install -r requirements.txt
echo
echo "Bip bip Bop Biiip, env setup complete"