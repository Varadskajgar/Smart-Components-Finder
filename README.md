# Smart Components Finder

A responsive electronic component inventory built with plain HTML, CSS and JavaScript. It is a static site ready for free GitHub Pages hosting.

## Run locally

Open [index.html](index.html) in a browser, or run `python -m http.server 8000` from the repository folder and visit http://localhost:8000.

## Edit the example inventory

Edit the `items` array near the bottom of `index.html` to change names, categories, example quantities, container numbers and storage locations. All quantities shown are sample data.

## Included features

- Search names, categories, descriptions and locations
- Category and container filters with a helpful empty result
- Google Shopping supplier search links
- Responsive table and mobile inventory cards
- Summary totals, container contents and / keyboard shortcut for search

## Future hardware work

`hardware/rfid-mapping.json` reserves a place for future RFID assignments. The ESP32 and RC522 reader are not connected. Database storage, inventory editing, live hardware updates and a deployment QR code are future features.

## Enable GitHub Pages

Open **Settings → Pages**, select **Deploy from a branch**, choose `main` and `/` (root), then save. GitHub Pages will publish the static site after deployment completes.
